#pragma once

#include "CoreMinimal.h"
#include "ItemInventory/BaseItem.h"
#include "NiagaraComponent.h"
#include "DomiFramework/WorldActorManage/ActorStateComponent.h"
#include "ItemDropped.generated.h"

class UWorldInstanceSubsystem;

UCLASS()
class DOMINIONPROTOCOL_API AItemDropped : public ABaseItem
{
	GENERATED_BODY()

public:
	AItemDropped();

protected:
	// 아이템 주변 아우라
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	UNiagaraComponent* AuraVFXComponent;

	// 아이템 주변 아우라로 사용할 나이아가라 시스템 에셋
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	class UNiagaraSystem* AuraNiagaraSystem;

	// 상태 컴포넌트(인스턴스 데이터 연동)
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UActorStateComponent> ActorStateComponent;

	//얻을 아이템 클래스
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Data", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ABaseItem> ItemSubclassToAward;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Data", meta = (AllowPrivateAccess = "true"))
	int32 QuantityToAward=1;

	virtual void BeginPlay() override;

	virtual void Interact_Implementation(AActor* Interactor) override;
};
