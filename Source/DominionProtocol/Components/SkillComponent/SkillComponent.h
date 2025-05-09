#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Util/GameTagList.h"
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
	TMap<FGameplayTag, FSkillGroup> SkillGroupsMap;

private:
	// 최근에 실행했던 스킬
	UBaseSkill* CurrentSkill = nullptr;
};