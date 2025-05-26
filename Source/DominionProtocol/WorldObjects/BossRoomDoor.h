// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WorldObjects/Door.h"
#include "Interface/StoryDependentInterface.h"
#include "DominionProtocol/Util/GameTagList.h"
#include "BossRoomDoor.generated.h"

UCLASS()
class DOMINIONPROTOCOL_API ABossRoomDoor : public ADoor, public IStoryDependentInterface
{
	GENERATED_BODY()

public:
	ABossRoomDoor();

protected:
	virtual void OpenDoor(float Value) override;
	virtual void Interact_Implementation(AActor* Interactor) override;
	virtual void OnStoryStateUpdated_Implementation(EGameStoryState NewState) override;

private:
	void StartDissolve();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Door")
	UStaticMeshComponent* SecondDoorMesh;

	UPROPERTY()
	USceneComponent* LeftDoorPivot;

	UPROPERTY()
	USceneComponent* RightDoorPivot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "key")
	FGameplayTag RequiredKey;


};
