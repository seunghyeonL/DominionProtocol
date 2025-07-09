#pragma once

#include "CoreMinimal.h"
#include "AI/AICharacters/BossMonster/BaseBossEnemy.h"
#include "Boss5Enemy.generated.h"

UCLASS()
class DOMINIONPROTOCOL_API ABoss5Enemy : public ABaseBossEnemy
{
	GENERATED_BODY()
	
public:
	ABoss5Enemy();

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	TObjectPtr<UChildActorComponent> HeadArmComponent;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	TObjectPtr<UChildActorComponent> RotateArmComponent;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	TObjectPtr<UChildActorComponent> ManipulatorTargeterComponent;
};