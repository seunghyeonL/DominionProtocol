// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss2Circling.h"
#include "Components/SkillComponent/SkillComponent.h"
#include "GameFramework/Character.h"
#include "Slate/SGameLayerManager.h"
#include "Util/DebugHelper.h"

UBoss2Circling::UBoss2Circling()
{
	SkillTag = SkillTags::Boss2Circling;
}

void UBoss2Circling::Circling() const
{
	const FVector MoveDirection = OwnerCharacter->GetActorRightVector();
	OwnerCharacter->AddMovementInput(MoveDirection, CirclingDirection);
}

void UBoss2Circling::EndSkill()
{
	// 캐릭터 이동 타이머 종료
	bIsCircling = false;
	USkillComponent* SkillComponent = OwnerCharacter->FindComponentByClass<USkillComponent>();
	check(SkillComponent);
	SkillComponent->EndSkill();
}

void UBoss2Circling::Execute()
{
	if (IsValid(OwnerCharacter))
	{
		// 이동 방향 결정
		CirclingDirection = FMath::RandBool() ? 0.5 : -0.5;
		Debug::Print(FString::Printf(TEXT("Direction: %f"),CirclingDirection));
		const float RandomTime = FMath::RandRange(MinTime, MaxTime);

		bIsCircling = true;
		
		// 랜덤 시간 후 스킬 종료
		GetWorld()->GetTimerManager().SetTimer(EndSkillTimerHandle, this, &UBoss2Circling::EndSkill, RandomTime, false);
	}
	else
	{
		Debug::PrintError("UBoss2Circling::Execute , InValid OwnerCharacter");
	}
}

void UBoss2Circling::Tick(float DeltaTime)
{
	if (bIsCircling)
	{
		// 캐릭터 바라보게 회전
		if (const AActor* TargetActor = UGameplayStatics::GetPlayerPawn(GetWorld(), 0))
		{
			const FVector Direction = (TargetActor->GetActorLocation() - OwnerCharacter->GetActorLocation()).GetSafeNormal();
			FRotator LookRotation = FRotationMatrix::MakeFromX(Direction).Rotator();
			LookRotation.Pitch = 0.f;
			LookRotation.Roll = 0.f;
			OwnerCharacter->SetActorRotation(LookRotation);
		}
		
		Circling();
	}
}




