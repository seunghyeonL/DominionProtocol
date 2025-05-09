#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Util/GameTagList.h"
#include "Util/BattleDataTypes.h"
#include "Components/SkillComponent/SkillComponentInitializeData.h"
#include "SkillComponent.generated.h"

class UBaseSkill;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DOMINIONPROTOCOL_API USkillComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USkillComponent();

	virtual void InitializeComponent() override;

	void InitializeSkillComponent(const FSkillComponentInitializeData& InitializeData);

	void ExecuteSkill(const FGameplayTag& SkillGroupTag);

	UBaseSkill* GetCurrentSkill() const { return CurrentSkill; }
	void SetCurrentSkill(UBaseSkill* Skill) { CurrentSkill = Skill; }

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill")
	TMap<FGameplayTag, FSkillGroup> SkillGroupMap;
	
	// 실행중인 스킬
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill")
	UBaseSkill* CurrentSkill = nullptr;
};