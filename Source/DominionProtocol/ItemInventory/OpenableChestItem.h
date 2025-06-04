#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/InteractableInterface.h"
#include "Components/BoxComponent.h"
#include "OpenableChestItem.generated.h"

class UStaticMeshComponent;
class USoundCue;
class UAnimMontage;
class ADomiCharacter;
class UCurveFloat;
class UTimelineComponent;

UCLASS()
class DOMINIONPROTOCOL_API AOpenableChestItem : public AActor, public IInteractableInterface
{
	GENERATED_BODY()
	
public:
	AOpenableChestItem();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Interact_Implementation(AActor* Interactor) override;
	virtual FText GetInteractMessage_Implementation() const override;

protected:
    UPROPERTY(VisibleAnywhere, Category = "Components")
    UStaticMeshComponent* BottomMesh; // 아이템 박스 하단

    UPROPERTY(VisibleAnywhere, Category = "Components")
    UStaticMeshComponent* TopMesh; //아이템 박스 상단

    UPROPERTY(VisibleAnywhere, Category = "Components")
    UBoxComponent* InteractionVolume;

    UPROPERTY(EditAnywhere, Category = "Item Spawning")
    TArray<TSubclassOf<AActor>> ItemClassesToSpawn; // 스폰할 아이템 클래스들의 배열

    UPROPERTY(EditAnywhere, Category = "Item Spawning")
    FVector SpawnOffset; 

    UPROPERTY(EditAnywhere, Category = "Animations")
    UCurveFloat* OpenCurve;

    UPROPERTY(EditAnywhere, Category = "Sound")
    USoundCue* OpenSound; //열림 사운드

    UPROPERTY(BlueprintReadOnly, Category = "State")
    bool bHasBeenOpened; // 박스가 이미 열렸는지 여부

private:
    UPROPERTY()
    UTimelineComponent* OpenTimeline;

    void PlayOpeningAnimation();

    UFUNCTION()
    void OpenChestUpdate(float Alpha);

    /*UFUNCTION()
    void OpenChestFinished();*/

    void PlayOpenSound(); // 사운드 재생 함수

    UFUNCTION()
    void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
    void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


};
