#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "ItemData.h"
#include "GameplayTagContainer.h"
#include "BaseItem.generated.h"

UCLASS()
class DOMINIONPROTOCOL_API ABaseItem : public AActor
{
	GENERATED_BODY()
	
public:	
	ABaseItem();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item Data")
	FName ItemDataRowName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Data")
	UDataTable* ItemDataTable;

	// 획득 로직 (상호작용)
	/*UFUNCTION(BlueprintCallable)
	virtual void Interact(APawn* InteractingPawn);*/
	// 오버랩 획득 로직 (필요에 따라)
	/*UFUNCTION()
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);*/

public:
	// 아이템 데이터 반환 (데이터 테이블에서 로드)
	FItemData* GetItemData()const;
	// 아이템 태그 반환
	/*UFUNCTION(BlueprintPure)
	FGameplayTag GetItemGameplayTag() const;*/
};
