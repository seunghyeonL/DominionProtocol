// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SkillComponent/Skills/SkillData.h"
#include "GameFramework/GameState.h"
#include "BaseGameState.generated.h"

class USoundInstanceSubsystem;

UCLASS()
class DOMINIONPROTOCOL_API ABaseGameState : public AGameState
{
	GENERATED_BODY()

public:
	ABaseGameState();

	FSkillData* GetSkillData(const FGameplayTag SkillTag) const;
	
protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	USoundInstanceSubsystem* SoundSubsystem;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill DataTable", meta = (AllowPrivateAccess = "true"))
	UDataTable* SkillDataTable;
};
