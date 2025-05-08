#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseItem.h"
#include "ItemSpawner.generated.h"

class UBoxComponent;
class USceneComponent;
class ABaseItem;
class AItemSpawnPoint;

UCLASS()
class DOMINIONPROTOCOL_API AItemSpawner : public AActor
{
	GENERATED_BODY()
	
public:
	AItemSpawner();

protected:
	virtual void BeginPlay() override;

	UPROPERTY()
	TArray<AItemSpawnPoint*> SpawnPoints;

	UPROPERTY(EditAnywhere, Category = "Spawn")
	TArray<TSubclassOf<class ABaseItem>> ItemClasses;

	UPROPERTY(EditAnywhere, Category = "Spawn")
	float RespawnTime = 3.0f;

	TMap<AItemSpawnPoint*, ABaseItem*> SpawnedItems;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UBoxComponent> SpawnArea;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<USceneComponent> SceneRoot;

	UFUNCTION(BlueprintCallable, Category = "Spawn")
	void SpawnItems();
	UFUNCTION(BlueprintCallable, Category = "Spawn")
	void RespawnItem(AItemSpawnPoint* SpawnPoint);
	UFUNCTION(BlueprintCallable, Category = "Spawn")
	void OnItemDestroyed(AActor* DestroyedActor);

};
