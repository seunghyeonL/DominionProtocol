#include "Components/SkillComponent/Skills/SkillObject/Portal.h"
#include "Util/DebugHelper.h"
#include "Components/SceneComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/SkillComponent/Skills/MagicSkill/MagicTeleportSkill.h"

APortal::APortal()
{
	StartPoint = CreateDefaultSubobject<USceneComponent>(TEXT("StartPoint"));
	EndPoint = CreateDefaultSubobject<USceneComponent>(TEXT("EndPoint"));
	MovePoint = CreateDefaultSubobject<USceneComponent>(TEXT("MovePoint"));
	BlueAura = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("BlueAura"));
	RedAura = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("RedAura"));
	
	RootComponent = StartPoint;

	EndPoint->SetupAttachment(RootComponent);
	MovePoint->SetupAttachment(RootComponent);

	BlueAura->SetupAttachment(RootComponent);
	RedAura->SetupAttachment(RootComponent);

	StartPoint->SetRelativeLocation(FVector(0.f, 0.f, 500.f));
	EndPoint->SetRelativeLocation(FVector(1000.f, 0.f, 0.f));

	static ConstructorHelpers::FObjectFinder<UParticleSystem> AuraAsset1(TEXT("/Game/FXVarietyPack/Particles/P_ky_healAura.P_ky_healAura"));
	if (AuraAsset1.Succeeded())
	{
		BlueAura->SetTemplate(AuraAsset1.Object);
		BlueAura->bAutoActivate = true;
	}
	else
	{
		Debug::PrintError(TEXT("Portal:: Blue Aura Asset's location is not assigned"));
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem> AuraAsset2(TEXT("/Game/FXVarietyPack/Particles/P_ky_magicCircle1.P_ky_magicCircle1"));
	if (AuraAsset2.Succeeded())
	{
		RedAura->SetTemplate(AuraAsset2.Object);
		RedAura->bAutoActivate = true;
	}
	else
	{
		Debug::PrintError(TEXT("Portal:: RedAura Asset's location is not assigned"));
	}
}

void APortal::DestroyPortal()
{
	SkillOwner->SetReadyToTeleport(false);
	SkillOwner->SetCanTeleport(false);

	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	SetActorTickEnabled(false);
	Destroy();
}