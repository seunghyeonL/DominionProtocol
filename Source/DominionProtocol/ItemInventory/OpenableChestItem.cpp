#include "ItemInventory/OpenableChestItem.h"
#include "DominionProtocol/Player/Characters/DomiCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Components/TimelineComponent.h"
#include "Curves/CurveFloat.h"
#include "DomiFramework/WorldActorManage/ActorStateComponent.h"
#include "Util/DebugHelper.h"
#include "Util/GameTagList.h"


AOpenableChestItem::AOpenableChestItem()
{
 	PrimaryActorTick.bCanEverTick = false;

    BottomMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BottomMesh"));
    RootComponent = BottomMesh; 

    TopMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TopMesh")); // ✨ TopMesh로 변경
    TopMesh->SetupAttachment(RootComponent);

    // InteractionVolume 생성 및 설정 추가
    InteractionVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractionVolume"));
    InteractionVolume->SetupAttachment(RootComponent);
    InteractionVolume->SetCollisionEnabled(ECollisionEnabled::QueryOnly); // 쿼리만 가능하게 설정
    InteractionVolume->SetCollisionResponseToAllChannels(ECR_Ignore); // 기본적으로 모든 채널 무시
    InteractionVolume->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECR_Overlap); // Pawn 채널과만 오버랩

    InteractionVolume->SetGenerateOverlapEvents(true);

    //StateComponent 추가
    ActorStateComponent = CreateDefaultSubobject<UActorStateComponent>(TEXT("ActorStateComponent"));
    ActorStateComponent->SetGameplayTag(WorldActorTags::OpenableChestItem);
    
    // 오버랩 이벤트 바인딩 추가
    InteractionVolume->OnComponentBeginOverlap.AddDynamic(this, &AOpenableChestItem::OnOverlapBegin);
    InteractionVolume->OnComponentEndOverlap.AddDynamic(this, &AOpenableChestItem::OnOverlapEnd);

    // 기본 설정
    BottomMesh->SetCollisionProfileName(TEXT("BlockAllDynamic"));
    TopMesh->SetCollisionProfileName(TEXT("BlockAllDynamic"));
    bHasBeenOpened = false; 
    SpawnOffset = FVector(0, 0, 50.0f);

    OpenTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("OpenTimeline"));
}

// Called when the game starts or when spawned
void AOpenableChestItem::BeginPlay()
{
	Super::BeginPlay();
	
    if (!BottomMesh->GetStaticMesh())
    {
        Debug::Print(FString::Printf(TEXT("Warning: Bottom '%s' has no StaticMesh assigned!"), *GetName()), FColor::Yellow);
    }
    if (!TopMesh->GetStaticMesh())
    {
        Debug::Print(FString::Printf(TEXT("Warning: TopMesh '%s' has no StaticMesh assigned!"), *GetName()), FColor::Yellow);
    }
    if (OpenCurve)
    {
        FOnTimelineFloat UpdateFunction;
        UpdateFunction.BindDynamic(this, &AOpenableChestItem::OpenChestUpdate);
        OpenTimeline->AddInterpFloat(OpenCurve, UpdateFunction);

        
    }
    else
    {
        Debug::Print(FString::Printf(TEXT("Error: OpenCurve is not assigned for Chest '%s'!"), *GetName()), FColor::Red);
    }

    if (ActorStateComponent->GetActorData().bIsChestOpened)
    {
        PlayOpeningAnimation();
    }
}

void AOpenableChestItem::Interact_Implementation(AActor* Interactor)
{
    if (bHasBeenOpened || ActorStateComponent->GetActorData().bIsChestOpened)
    {
        Debug::Print(TEXT("This Item Box has already been opened."), FColor::Yellow);
        return;
    }

    Debug::Print(FString::Printf(TEXT("Item Box '%s' interacted by %s!"), *GetName(), *Interactor->GetName()), FColor::Green);
    bHasBeenOpened = true;
    PlayOpeningAnimation();
    PlayOpenSound();
    ActorStateComponent->SwitchStateAndUpdateInstance(WorldActorTags::OpenableChestItem, 0);
    
    if (ItemClassesToSpawn.Num() > 0)
    {
        // 배열에서 무작위로 하나의 아이템 클래스 선택
        int32 RandomIndex = FMath::RandRange(0, ItemClassesToSpawn.Num() - 1);
        TSubclassOf<AActor> ChosenItemClass = ItemClassesToSpawn[RandomIndex];

        if (ChosenItemClass)
        {
            FVector SpawnLocation = GetActorLocation() + SpawnOffset;
            FRotator SpawnRotation = FRotator::ZeroRotator;

            AActor* SpawnedItem = GetWorld()->SpawnActor<AActor>(ChosenItemClass, SpawnLocation, SpawnRotation);

            if (SpawnedItem)
            {
                SpawnedItem->SetOwner(this);
                Debug::Print(FString::Printf(TEXT("Spawned item: %s from Item Box."), *SpawnedItem->GetName()), FColor::Blue);
            }
            else
            {
                Debug::Print(TEXT("Failed to spawn item! Check item class settings."), FColor::Red);
            }
        }
        else
        {
            Debug::Print(TEXT("Chosen item class is null in ItemClassesToSpawn."), FColor::Yellow);
        }
    }
    else
    {
        Debug::Print(TEXT("ItemClassesToSpawn array is empty. No item will be spawned."), FColor::Yellow);
    }
    ADomiCharacter* InteractingDomiCharacter = Cast<ADomiCharacter>(Interactor);
    if (InteractingDomiCharacter && InteractingDomiCharacter->IsLocallyControlled())
    {
        InteractingDomiCharacter->RemoveInteractableActor(this);
    }
}

FText AOpenableChestItem::GetInteractMessage_Implementation() const
{
    return FText::FromString(TEXT("상자 열기"));
}

void AOpenableChestItem::OpenChestUpdate(float Alpha)
{
    if (TopMesh)
    {
        // Y축으로 0도에서 180도까지 회전 (피치, X축 회전)
        FRotator NewRotation = FRotator(0.0f, 0.0f, FMath::Lerp(0.0f, 120.0f, Alpha));
        TopMesh->SetRelativeRotation(NewRotation);
    }
}

void AOpenableChestItem::PlayOpeningAnimation()
{
    if (OpenTimeline)
    {
        OpenTimeline->PlayFromStart();
        Debug::Print(TEXT("Chest opening animation started."), FColor::Cyan);
    }
}

//사운드 출력
void AOpenableChestItem::PlayOpenSound()
{
    if (OpenSound)
    {
        UGameplayStatics::PlaySoundAtLocation(this, OpenSound, GetActorLocation());
    }
    else
    {
        Debug::Print(TEXT("OpenSound is null. Cannot play open sound."), FColor::Yellow);
    }
}

void AOpenableChestItem::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    ADomiCharacter* PlayerCharacter = Cast<ADomiCharacter>(OtherActor);
    if (PlayerCharacter && PlayerCharacter->IsLocallyControlled())
    {
        PlayerCharacter->AddInteractableActor(this);
        Debug::Print(FString::Printf(TEXT("Item Box '%s'와 오버랩 시작: '%s'를 InteractableObject로 설정."), *GetName(), *PlayerCharacter->GetName()), FColor::Blue);
    }
}

void AOpenableChestItem::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    ADomiCharacter* PlayerCharacter = Cast<ADomiCharacter>(OtherActor);
    if (PlayerCharacter && PlayerCharacter->IsLocallyControlled())
    {
        PlayerCharacter->RemoveInteractableActor(this);
        Debug::Print(FString::Printf(TEXT("Item Box '%s'와 오버랩 종료: '%s'를 InteractableObject에서 해제."), *GetName(), *PlayerCharacter->GetName()), FColor::Blue);
    }
}

