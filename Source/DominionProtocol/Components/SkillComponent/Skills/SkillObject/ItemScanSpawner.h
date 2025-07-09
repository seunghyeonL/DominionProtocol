#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemScanSpawner.generated.h"

UCLASS()
class DOMINIONPROTOCOL_API AItemScanSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	AItemScanSpawner();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};