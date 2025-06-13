#include "Helper.h"
#include "Kismet/GameplayStatics.h"
#include "Animation/AnimInstance.h"
#include "WorldObjects/DialogueManager.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

AHelper::AHelper()
{
	PrimaryActorTick.bCanEverTick = true;

	SetRootComponent(Cast<USceneComponent>(GetCapsuleComponent()));

	Hair = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("HairMesh"));
	Hair->SetupAttachment(GetMesh());

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void AHelper::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AHelper::BeginPlay()
{
	Super::BeginPlay();
}

void AHelper::SetDialogueManager(UDialogueManager* InManager)
{
	DialogueManager = InManager;
}

void AHelper::Appear(const FVector& SpawnLocation)
{
	SetActorLocation(SpawnLocation + FVector(100.f, 100.f, 100.f));
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

	const FVector LaunchDirection = GetActorForwardVector() * 50.f + FVector(10.f, 10.f, 50.f);
	LaunchCharacter(LaunchDirection, true, true);

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
	if (DisappearMontage)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		UAnimInstance* HeadAnim = Hair->GetAnimInstance();
		if (AnimInstance)
		{
			AnimInstance->Montage_Play(DisappearMontage, 1.f);
			HeadAnim->Montage_Play(DisappearMontage, 1.f);

			FOnMontageEnded EndDelegate;
			EndDelegate.BindUObject(this, &AHelper::OnDisappearMontageEnded);
			AnimInstance->Montage_SetEndDelegate(EndDelegate, DisappearMontage);
		}
	}
	else
	{
		Destroy();
	}
}

void AHelper::OnDisappearMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
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