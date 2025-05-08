#pragma once

#include "CoreMinimal.h"
#include "Components/SkillComponent/SkillComponentBase.h"
#include "AISkillComponent.generated.h"

// class ADomiBaseEnemy;

UCLASS()
class DOMINIONPROTOCOL_API UAISkillComponent : public USkillComponentBase
{
	GENERATED_BODY()

public:

	UAISkillComponent();
	
	// 패턴별 스킬 그룹 관리
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = true))
	TMap<FGameplayTag, FSkillGroup> Patterns;

    //// AI 전용 패턴 기반 스킬 실행
    //UFUNCTION(BlueprintCallable, Category = "AI Skills")
    //void ExecutePatternSkill(FGameplayTag PatternTag);

    //// 랜덤 패턴 선택 및 실행
    //UFUNCTION(BlueprintCallable, Category = "AI Skills")
    //void ExecuteRandomPattern();

	// 스킬 실행 함수
	virtual void ExecuteSkill(FGameplayTag SkillGroupTag) override;

protected:
    virtual void BeginPlay() override;

    // 패턴 초기화
    virtual void InitializePatterns();
};