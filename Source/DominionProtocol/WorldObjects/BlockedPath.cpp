#include "WorldObjects/BlockedPath.h"
#include "Components/BoxComponent.h"
#include "DomiFramework/GameInstance/DomiGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Player/Characters/DomiCharacter.h"
#include "Components/ItemComponent/ItemComponent.h"
#include "WorldObjects/DialogueManager.h"
#include "UI/UIInGame/NewDialogueWidget.h"
#include "Util/DebugHelper.h"

ABlockedPath::ABlockedPath()
	:bIsBlocking(true),
	DialogueID(TEXT("BlockedPath"))
{
	PrimaryActorTick.bCanEverTick = false;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	CollisionBox->SetupAttachment(RootComponent);
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	BlockingBox = CreateDefaultSubobject<UBoxComponent>(TEXT("BlockingBox"));
	BlockingBox->SetupAttachment(RootComponent);
	BlockingBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	BlockingBox->SetCollisionResponseToAllChannels(ECR_Block);

	PathEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("PathEffect"));
	PathEffect->SetupAttachment(RootComponent);
	PathEffect->SetAutoActivate(false); 

	PathEffect->SetRelativeLocation(FVector::ZeroVector);
	PathEffect->SetWorldLocation(GetActorLocation() + FVector(0, 0, 0.f));
	PathEffect->SetWorldScale3D(FVector(1.f));
}

void ABlockedPath::OnStoryStateUpdated_Implementation(EGameStoryState NewState)
{
	Debug::Print(TEXT("ABlockedPath::OnStoryStateUpdated_Implementation"));
	if (NewState > RequiredStoryState)
	{
		TryOpen();
	}
}

void ABlockedPath::BeginPlay()
{
	Super::BeginPlay();

	if (!CollisionBox || !PathEffect)
	{
		Debug::Print(TEXT("ABlockedPath::BeginPlay - 구성 요소가 초기화되지 않음!"));
		return;
	}

	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ABlockedPath::OnOverlapBegin);
	CollisionBox->OnComponentEndOverlap.AddDynamic(this, &ABlockedPath::OnOverlapEnd);

	if (UDomiGameInstance* GI = Cast<UDomiGameInstance>(UGameplayStatics::GetGameInstance(this)))
	{
		GI->OnStoryStateChanged.AddDynamic(this, &ABlockedPath::OnStoryStateUpdated);
	}

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
		{
			if (PathEffect && NiagaraTemplate)
			{
				PathEffect->SetAsset(NiagaraTemplate);
				PathEffect->ReinitializeSystem();
				PathEffect->SetVisibility(true, true);
				PathEffect->Activate();
			}
		}, 0.1f, false);

	
	//TryOpen();
}

void ABlockedPath::TryOpen()
{
	Debug::Print(TEXT("ABlockedPath: TryOpen"));
	if (!IsValid(CollisionBox))
	{
		Debug::Print(TEXT("ABlockedPath: CollisionBox is null"));
		return;
	}

	if (!IsValid(PathEffect))
	{
		Debug::Print(TEXT("ABlockedPath: PathEffect is null"));
		return;
	}
	UDomiGameInstance* GameInstance = Cast<UDomiGameInstance>(UGameplayStatics::GetGameInstance(this));
	if (GameInstance && GameInstance->GetCurrentGameStoryState() >= RequiredStoryState)
	{
		Debug::Print(TEXT("ABlockedPath: 조건 충족, 열기"));
		bIsBlocking = false;
		BlockingBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		CollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		PathEffect->Deactivate();
		PathEffect->DestroyComponent();
	}
	else
	{
		Debug::Print(TEXT("ABlockedPath: 조건 미충족, 막힘 유지"));
		DialogueManager->TryStartDialogueByID(DialogueID);
	}
}
void ABlockedPath::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Debug::Print(TEXT("ABlockedPath: Overlap"));

	if (!IsValid(OtherActor) || !OtherActor->ActorHasTag("Player"))
	{
		Debug::Print(TEXT("Not Player"));
		return;
	}
	/*
	UDomiGameInstance* GI = Cast<UDomiGameInstance>(UGameplayStatics::GetGameInstance(this));
	if (!GI || GI->GetCurrentGameStoryState() != RequiredStoryState)
	{
		Debug::Print(TEXT("Not RequiredStoryState"));
		return;
	}*/

	ADomiCharacter* PlayerCharacter = Cast<ADomiCharacter>(OtherActor);
	ensure(PlayerCharacter);
	CachedCharacter = PlayerCharacter;

	PlayerCharacter->AddInteractableActor(this);
}

void ABlockedPath::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (IsValid(OtherActor) && OtherActor == CachedCharacter)
	{
		ADomiCharacter* PlayerCharacter = Cast<ADomiCharacter>(OtherActor);
		ensure(PlayerCharacter);

		CachedCharacter = nullptr;
		PlayerCharacter->RemoveInteractableActor(this);
	}
	else
	{
		Debug::Print(TEXT("ABlockedPath::OnOverlapEnd : OtherActor Is not PlayerCharacter"));
	}
}

void ABlockedPath::StartDialogue(const FString& ID)
{/*
	UDialogueManager* DialogueManager = NewObject<UDialogueManager>(this);

	// UI 바인딩
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0))
	{
		if (UNewDialogueWidget* HUD = Cast<UNewDialogueWidget>(PC->GetHUD()))
		{
			if (UNewDialogueWidget* DialogueWidget = HUD->GetDialogueWidget())
			{
				DialogueWidget->BindDialogueDelegate(DialogueManager);
			}
		}
	}

	DialogueManager->TryStartDialogueByID(DialogueID);*/
}

void ABlockedPath::Interact_Implementation(AActor* Interactor)
{
	ADomiCharacter* PlayerCharacter = Cast<ADomiCharacter>(Interactor);
	if (!PlayerCharacter || !bIsBlocking) return;

	if (!DialogueManager)
	{
		DialogueManager = NewObject<UDialogueManager>(this);
		OnCreateDialogueManager.Broadcast(DialogueManager);
	}

	UItemComponent* PlayerItemComponent = PlayerCharacter->FindComponentByClass<UItemComponent>();
	if (!IsValid(PlayerItemComponent) || !PlayerItemComponent->HasItem(RequiredKey, 1))
	{
		Debug::Print(TEXT("ABlockedPath::Item Not Found"));

		DialogueManager->TryStartDialogueByID(DialogueID);
		return;
	}

	UDomiGameInstance* GI = Cast<UDomiGameInstance>(UGameplayStatics::GetGameInstance(this));
	if (!GI || GI->GetCurrentGameStoryState()!= RequiredStoryState)
	{ 
		Debug::Print(TEXT("ABlockedPath::Not RequiredStoryState"));

		DialogueManager->TryStartDialogueByID(DialogueID);
		return; 
	}
		
	Debug::Print(TEXT("ABlockedPath::Item Check"));
	PlayerItemComponent->RemoveItem(RequiredKey, 1);
	GI->AdvanceStoryState();
	TryOpen();
}

FText ABlockedPath::GetInteractMessage_Implementation() const
{
	return FText::FromString(TEXT("확인하기"));
}