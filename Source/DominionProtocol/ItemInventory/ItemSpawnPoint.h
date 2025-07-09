#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseItem.h"
#include "ItemSpawnPoint.generated.h"

UCLASS()
class DOMINIONPROTOCOL_API AItemSpawnPoint : public AActor
{
	GENERATED_BODY()
	
public:
	AItemSpawnPoint();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void SpawnItem();

public:
	UPROPERTY(EditAnywhere, Category = "Spawn")
	TArray<TSubclassOf<ABaseItem>> ItemClasses;

	UPROPERTY(EditAnywhere, Category = "Spawn")
	FVector SpawnOffset = FVector::ZeroVector;

};
