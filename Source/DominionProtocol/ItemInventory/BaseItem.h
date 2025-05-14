#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "ItemData.h"
#include "Util/GameTagList.h"
#include "Interface/InteractableInterface.h"
#include "BaseItem.generated.h"

UCLASS()
class DOMINIONPROTOCOL_API ABaseItem : public AActor, public IInteractableInterface
{
	GENERATED_BODY()
	
public:	
	ABaseItem();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item Data")
	FGameplayTag ItemTag;

	//아이템 데이터테이블
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Data")
	UDataTable* ItemDataTable;

	// 획득 로직 (상호작용)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UBoxComponent* InteractionVolume;

	UFUNCTION(BlueprintCallable)
	virtual void InteractItem(APawn* InteractingPawn);

	//상호작용 인터페이스
	virtual void Interact_Implementation(AActor* Interactor) override;
	virtual FText GetInteractMessage_Implementation() const override;
	
	//상호작용을 위한 오버랩 Begin
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	//상호작용을 위한 오버랩 End
	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:
	// 아이템 데이터 반환 (데이터 테이블에서 로드)
	FItemData* GetItemData()const;
	// 아이템 태그 반환
	UFUNCTION(BlueprintPure)
	FGameplayTag GetItemGameplayTag() const;

	virtual void BeginPlay()override;
};
