#include "WorldObjects/BossRoomDoor.h"
#include "Components/ItemComponent/ItemComponent.h"
#include "Player/Characters/DomiCharacter.h"
#include "Util/DebugHelper.h"
#include "DomiFramework/GameInstance/DomiGameInstance.h"
#include "Kismet/GameplayStatics.h"

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



void ABossRoomDoor::BeginPlay()
{
    Super::BeginPlay();

    if (UDomiGameInstance* GI = Cast<UDomiGameInstance>(UGameplayStatics::GetGameInstance(this)))
    {
        GI->OnStoryStateChanged.AddDynamic(this, &ABossRoomDoor::OnStoryStateUpdated);
    }
}

void ABossRoomDoor::OpenDoor(float Value)
{
	DoorMesh->SetRelativeRotation(FRotator(0.f, DoorRotateAngle * Value, 0.f));
	SecondDoorMesh->SetRelativeRotation(FRotator(0.f, 180-DoorRotateAngle * Value, 0.f));
}

void ABossRoomDoor::Interact_Implementation(AActor* Interactor)
{
    if (!bOpenByLever && !bRequireKey)
    {
        Debug::Print(TEXT("ABossRoomDoor::Interact_Implementation: 레버도 아니고 키도 필요하지 않음 - 조건 미충족"));
        return;
    }

    if (bAutoOpenByStory)
    {
        Debug::Print(TEXT("ABossRoomDoor::Interact_Implementation: 스토리 상태로 자동 열리는 문 - 수동 상호작용 무시"));
        return;
    }

    if (!bIsDoorClosed)
    {
        Debug::Print(TEXT("ABossRoomDoor::Interact_Implementation: 문이 이미 열려 있음"));
        return;
    }

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

    if (bRequireKey)
    {
        if (PlayerItemComponent->HasItem(RequiredKey, 1))
        {
            Timeline.Play();
            StartDissolve();
            bIsDoorClosed = false;

            if (UDomiGameInstance* GI = Cast<UDomiGameInstance>(GetGameInstance()))
            {
                GI->SetCurrentGameStoryState(EGameStoryState::BattleWithBoss);
            }
        }
        else
        {
            Debug::Print(TEXT("ABossRoomDoor::Interact_Implementation: 열쇠 없음"));
        }
    }
    else if (bOpenByLever)
    {
        Timeline.Play();
        StartDissolve();
        bIsDoorClosed = false;
    }
}


void ABossRoomDoor::OnStoryStateUpdated_Implementation(EGameStoryState NewState)
{
    Debug::Print(FString::Printf(TEXT("문이 상태 [%d] 수신. 내 RequiredState = %d"), (int32)NewState, (int32)RequiredState));

    if (NewState == RequiredState)
    {
        StartDissolve();
    }
}

void ABossRoomDoor::StartDissolve()
{
    DoorMesh->SetVisibility(false);
    DoorMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    SecondDoorMesh->SetVisibility(false);
    SecondDoorMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}