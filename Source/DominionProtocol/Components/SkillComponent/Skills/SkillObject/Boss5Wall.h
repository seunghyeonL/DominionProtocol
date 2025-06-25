#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Boss5Wall.generated.h"

class ADomiCharacter;
class UBoxComponent;

UCLASS()
class DOMINIONPROTOCOL_API ABoss5Wall : public AActor
{
    GENERATED_BODY()

public:
    ABoss5Wall();
    virtual void Tick(float DeltaTime) override;
    void Push();

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    UStaticMeshComponent* WallMesh;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    UBoxComponent* PushTrigger;

    UFUNCTION()
    void OnPushTriggerBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
    void OnPushTriggerEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

    TWeakObjectPtr<ADomiCharacter> OverlappingCharacter;

private:
    bool bShouldPush;
    bool bShouldReturn;

    float PushSpeed;
    float PushProgress;

    FVector StartLocation;
    FVector EndLocation;

    float LastPushTime;
    float PushCooldown;
};
