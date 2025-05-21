#include "WorldObjects/BossRoomDoor.h"
#include "Components/ItemComponent/ItemComponent.h"
#include "Player/Characters/DomiCharacter.h"
#include "Util/DebugHelper.h"
#include "DomiFramework/GameInstance/DomiGameInstance.h"

ABossRoomDoor::ABossRoomDoor()
{
    LeftDoorPivot = CreateDefaultSubobject<USceneComponent>(TEXT("LeftDoorPivot"));
    LeftDoorPivot->SetupAttachment(SceneComp);
    LeftDoorPivot->SetRelativeLocation(FVector(-100.f, 0.f, 0.f));

    DoorMesh->SetupAttachment(LeftDoorPivot);

    RightDoorPivot = CreateDefaultSubobject<USceneComponent>(TEXT("RightDoorPivot"));
    RightDoorPivot->SetupAttachment(SceneComp);
    RightDoorPivot->SetRelativeLocation(FVector(100.f, 0.f, 0.f));

    SecondDoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SecondDoorMesh"));
    SecondDoorMesh->SetupAttachment(RightDoorPivot);
    SecondDoorMesh->SetCollisionProfileName(TEXT("BlockAll"));
}

void ABossRoomDoor::OpenDoor(float Value)
{
	DoorMesh->SetRelativeRotation(FRotator(0.f, DoorRotateAngle * Value, 0.f));
	SecondDoorMesh->SetRelativeRotation(FRotator(0.f, 180-DoorRotateAngle * Value, 0.f));
}

void ABossRoomDoor::Interact_Implementation(AActor* Interactor)
{
    ADomiCharacter* PlayerCharacter = Cast<ADomiCharacter>(Interactor);
    if (!IsValid(PlayerCharacter))
    {
        Debug::Print(TEXT("ABossRoomDoor::Interact_Implementation: Interactor is not a valid PlayerCharacter."));
        return;
    }

    UItemComponent* PlayerItemComponent = PlayerCharacter->FindComponentByClass<UItemComponent>();
    if (!IsValid(PlayerItemComponent))
    {
        Debug::Print(TEXT("ABossRoomDoor::Interact_Implementation: Player's ItemComponent not found!"));
        return;
    }

    DoorMesh->SetCollisionProfileName(TEXT("NoCollision"));
    if (bIsDoorClosed)
    {
        if (PlayerItemComponent->HasItem(RequiredKey, 1))
        {
            Timeline.Play();
            StartDissolve();
            bIsDoorClosed = !bIsDoorClosed;

            if (UDomiGameInstance* GI = Cast<UDomiGameInstance>(GetGameInstance()))
            {
                GI->SetCurrentGameStoryState(EGameStoryState::BossDoorOpened);
            }
        }
        else
        {
            Debug::Print(TEXT("ABossRoomDoor::Interact_Implementation: Key X"));
        }
    }
    else
    {
        Debug::Print(TEXT("ABossRoomDoor::Interact_Implementation: Door Already Opened"));
    }
    
}

void ABossRoomDoor::OnStoryStateUpdated_Implementation(EGameStoryState NewState)
{
    if (NewState == EGameStoryState::BossDoorOpened)
    {
        StartDissolve();
    }
    else
    {
    }
}

void ABossRoomDoor::StartDissolve()
{
    DoorMesh->SetVisibility(false);
    DoorMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    SecondDoorMesh->SetVisibility(false);
    SecondDoorMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}