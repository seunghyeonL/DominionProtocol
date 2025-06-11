#include "AnimNotify_ItemScanning.h"
#include "Util/DebugHelper.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Components/SkillComponent/SkillComponent.h"
#include "Components/SkillComponent/Skills/SkillObject/ItemScanSpawner.h"
#include "Components/SkillComponent/Skills/MagicSkill/MagicItemScanningSkill.h"

void UAnimNotify_ItemScanning::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	OwnerCharacter = MeshComp->GetOwner();
	if (!IsValid(OwnerCharacter)) return;

	USkillComponent* SkillComponent = OwnerCharacter->FindComponentByClass<USkillComponent>();
	if (!IsValid(SkillComponent)) return;

	UBaseSkill* CurrentSkill = SkillComponent->GetCurrentSkill();
	if (!IsValid(CurrentSkill)) return;

	ItemScanningSkill = Cast<UMagicItemScanningSkill>(CurrentSkill);

	UKismetMaterialLibrary::SetScalarParameterValue(GetWorld(), ItemScanningSkill->ScanParameterCollection, FName("Radius"), 0.0f);
	
	ItemScanningSkill->IsStart = true;


	FString Path = TEXT("/Game/Blueprints/Skills/SkillObject/ItemScanning/ItemScanSpawner.ItemScanSpawner_C");
	UClass* LoadedClass = Cast<UClass>(StaticLoadObject(UClass::StaticClass(), nullptr, *Path));

	if (!LoadedClass)
	{
		Debug::PrintError(TEXT("Failed to load ItemScanSpawner class"));
		return;
	}

	FVector SpawnLocation = OwnerCharacter->GetActorLocation();
	FRotator SpawnRotation = FRotator::ZeroRotator;

	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	AItemScanSpawner* Spawner = GetWorld()->SpawnActor<AItemScanSpawner>(LoadedClass, SpawnLocation, SpawnRotation, Params);
}