#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Util/GameTagList.h"
#include "Util/BattleDataTypes.h"
#include "SkillComponentBase.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class DOMINIONPROTOCOL_API USkillComponentBase : public UActorComponent
{
	GENERATED_BODY()

public:	
	USkillComponentBase() {};

	virtual void ExecuteSkill(FGameplayTag SkillGroupTag) {};

	// SkillComponent를 가지고 있는 액터에서 초기화
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skills")
	TMap<FGameplayTag, FSkillGroup> SkillGroups;
};
