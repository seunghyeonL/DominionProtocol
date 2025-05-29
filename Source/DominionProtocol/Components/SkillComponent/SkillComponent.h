#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Util/GameTagList.h"
#include "Util/BattleDataTypes.h"
#include "Components/SkillComponent/SkillComponentInitializeData.h"
#include "SkillComponent.generated.h"

class UBaseSkill;

DECLARE_DELEGATE_OneParam(FOnSkillStart, FGameplayTag);
DECLARE_DELEGATE_OneParam(FOnSkillEnd, FGameplayTag);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DOMINIONPROTOCOL_API USkillComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USkillComponent();

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void InitializeComponent() override;

	void SetSkills(const FSkillComponentInitializeData& InitializeData);

	void ExecuteSkill(const FGameplayTag& SkillGroupTag);

	void EndSkill();

	void StopSkill();

	UBaseSkill* GetCurrentSkill() const { return CurrentSkill; }
	void SetCurrentSkill(UBaseSkill* Skill) { CurrentSkill = Skill; }

	FOnSkillStart OnSkillStart;
	FOnSkillEnd OnSkillEnd;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill")
	TMap<FGameplayTag, FSkillGroup> SkillGroupMap;
	
	// 실행중인 스킬
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill")
	UBaseSkill* CurrentSkill = nullptr;

private:
	// 콤보 스킬 리셋 딜레이
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill", meta = (AllowPrivateAccess = true))
	float ComboResetDelay;

	FTimerHandle ResetComboTimer;

	FGameplayTag CurrentSkillGroupTag;

	
};