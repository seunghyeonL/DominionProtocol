#include "Helper.h"
#include "Kismet/GameplayStatics.h"
#include "Animation/AnimInstance.h"
#include "WorldObjects/DialogueManager.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "ViewTarget.h"
#include "Kismet/KismetMathLibrary.h"
#include "Util/DebugHelper.h"

AHelper::AHelper()
{
	PrimaryActorTick.bCanEverTick = true;

	SetRootComponent(Cast<USceneComponent>(GetCapsuleComponent()));

	Hair = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("HairMesh"));
	Hair->SetupAttachment(GetMesh());

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	GetMesh()->bReceivesDecals = false;
	PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
}

void AHelper::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bIsFocussed)
	{
		FRotator Rot = UKismetMathLibrary::FindLookAtRotation(ViewTarget->GetActorLocation(), GetActorLocation());
		ViewTarget->SetActorRotation(Rot);
	}
	if (bAppearFinished)
	{
		FVector ToTarget = ViewTarget->GetActorLocation() - GetActorLocation();
		ToTarget.Z = 0.f;
		ToTarget.Normalize();
		AddMovementInput(ToTarget, 1.f);
	}
}

void AHelper::BeginPlay()
{
	Super::BeginPlay();
	ViewTarget = GetWorld()->SpawnActor<AViewTarget>(FVector::ZeroVector, FRotator::ZeroRotator, FActorSpawnParameters());
}

void AHelper::SetDialogueManager(UDialogueManager* InManager)
{
	DialogueManager = InManager;
}

void AHelper::Appear(const FVector& SpawnLocation, const FRotator& SpawnRotation)
{
	SetActorLocation(SpawnLocation + FVector(-20.f, -50.f, 10.f));
	SetActorRotation(SpawnRotation);
	SetActorHiddenInGame(false);

	UCapsuleComponent* Capsule = GetCapsuleComponent();
	if (Capsule)
	{
		Capsule->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	}

	if (AppearEffect)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			this,
			AppearEffect,
			GetActorLocation()
		);
	}

	//const FVector LaunchDirection = GetActorForwardVector() * 50.f + FVector(10.f, 10.f, 50.f);
	//LaunchCharacter(LaunchDirection, true, true);

	if (AppearMontage)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		UAnimInstance* HeadAnim = Hair->GetAnimInstance();
		if (AnimInstance)
		{
			AnimInstance->Montage_Play(AppearMontage, 1.f);
			HeadAnim->Montage_Play(AppearMontage, 1.f);

			FOnMontageEnded EndDelegate;
			EndDelegate.BindUObject(this, &AHelper::OnAppearMontageEnded);
			AnimInstance->Montage_SetEndDelegate(EndDelegate, AppearMontage);
		}
	}
}
void AHelper::Disappear()
{
	Debug::Print(TEXT("AHelper::Disappear()"));
	if (DisappearMontage)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		UAnimInstance* HeadAnim = Hair->GetAnimInstance();
		if (AnimInstance)
		{
			SetActorRotation(FRotator(0.f, 0.f, 100.f));

			AnimInstance->Montage_Play(DisappearMontage, 1.f);
			HeadAnim->Montage_Play(DisappearMontage, 1.f);

			FOnMontageEnded EndDelegate;
			EndDelegate.BindUObject(this, &AHelper::OnDisappearMontageEnded);
			AnimInstance->Montage_SetEndDelegate(EndDelegate, DisappearMontage);
			Debug::Print(TEXT("AHelper::Disappear() AnimInstance"));

			Destroy(); // 진행을 위한 임시 삭제
		}
	}
	else
	{
		Destroy();
		Debug::Print(TEXT("AHelper::Disappear() NoAnimInstance"));
	}
}

void AHelper::OnDisappearMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	Debug::Print(TEXT("AHelper::OnDisappearMontageEnded;pp"));
	Destroy();
}

void AHelper::OnAppearMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	UCapsuleComponent* Capsule = GetCapsuleComponent();
	if (Capsule)
	{
		Capsule->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
	}

	if (OnAppearFinishedCallback.IsBound())
	{
		OnAppearFinishedCallback.Execute();
	}
}

void AHelper::SetViewTargetLocAndRot(const FVector& InLocation, const FRotator& InRotation) const
{
	ViewTarget->SetActorLocation(InLocation);
	ViewTarget->SetActorRotation(InRotation);
}

void AHelper::SetViewToHelper()
{
	PC->SetViewTargetWithBlend(ViewTarget, 0.5f);
	bIsFocussed = true;
}

void AHelper::SetViewToPlayer()
{
	PC->SetViewTargetWithBlend(PC->GetPawn(), 0.5f);
	bIsFocussed = false;
}
