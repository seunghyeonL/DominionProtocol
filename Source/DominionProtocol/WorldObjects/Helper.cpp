#include "Helper.h"
#include "Kismet/GameplayStatics.h"
#include "Animation/AnimInstance.h"

AHelper::AHelper()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AHelper::BeginPlay()
{
	Super::BeginPlay();
}

void AHelper::Appear(const FVector& SpawnLocation)
{
	SetActorLocation(SpawnLocation);
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);

	if (AppearMontage)
	{
		/* // 등장 종료 후 dialogueManager가 종료 상태를 알기 위한 바인딩
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance)
		{
			FOnMontageEnded EndDelegate;
			EndDelegate.BindUObject(this, &AHelper::OnAppearMontageEnded);
			AnimInstance->Montage_SetEndDelegate(EndDelegate, AppearMontage);
		}
		*/
		PlayAnimMontage(AppearMontage);
	}
}
void AHelper::Disappear()
{
	if (DisappearMontage)
	{
		/*
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance)
		{
			FOnMontageEnded EndDelegate;
			EndDelegate.BindUObject(this, &AHelper::OnDisappearMontageEnded);
			AnimInstance->Montage_SetEndDelegate(EndDelegate, DisappearMontage);
		}
		*/
		PlayAnimMontage(DisappearMontage);
	}
	else
	{
		Destroy();
	}
}

void AHelper::OnDisappearMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	// DialogueManager->NotifyHelperDisppeared();
	Destroy();
}

void AHelper::OnAppearMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	// 등장 완료 후 다음 대사 진행을 DialogueManager에 알림
	// DialogueManager->NotifyHelperAppeared();
}