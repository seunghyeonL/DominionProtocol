#include "Components/SkillComponent/Skills/MagicSkill/MagicItemScanningSkill.h"
#include "Engine/World.h"
#include "Util/DebugHelper.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "Engine/PostProcessVolume.h"
#include "Materials/MaterialInterface.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Materials/MaterialParameterCollection.h"
#include "Components/SkillComponent/SkillComponent.h"

UMagicItemScanningSkill::UMagicItemScanningSkill()
{
	SkillTag = SkillTags::MagicItemScanningSkill;
	bIsBackupValid = false;
}

void UMagicItemScanningSkill::Execute()
{	
	FindAndSetPostProcessVolume();
	LoadScanMaterial();
	LoadParameterCollection();

	// 머티리얼 로드 후 즉시 적용
	if (PostProcessVolume && ScanMaterial)
	{
		// 원본 설정 백업
		BackupOriginalBlendables();
		SwapPostProcessMaterial();

		Super::Execute();
	}

	GetWorld()->GetTimerManager().SetTimer(ScanningTimerHandle, this, &UMagicItemScanningSkill::EndScannning, Duration, false);
}

void UMagicItemScanningSkill::FindAndSetPostProcessVolume()
{
	if (!GetWorld()) return;

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APostProcessVolume::StaticClass(), FoundActors);

	if (FoundActors.Num() > 0)
	{
		PostProcessVolume = Cast<APostProcessVolume>(FoundActors[0]);
		FString Msg = FString::Printf(TEXT("PostProcessVolume found: %s"),
			PostProcessVolume ? *PostProcessVolume->GetName() : TEXT("Cast Failed"));
		Debug::Print(Msg);
	}
	else
	{
		Debug::PrintLog(TEXT("UMagicItemScanningSkill::No PostProcessVolume found in the world"));
	}
}

void UMagicItemScanningSkill::LoadScanMaterial()
{
	// MI_Scan 로드
	FString MaterialPath = TEXT("/Game/Blueprints/Skills/SkillObject/ItemScanning/Materials/MI_Scan.MI_Scan");
	UMaterialInterface* LoadedMaterial = LoadObject<UMaterialInterface>(nullptr, *MaterialPath);

	if (LoadedMaterial)
	{
		ScanMaterial = LoadedMaterial;
		FString Msg = FString::Printf(TEXT("UMagicItemScanningSkill::Scan material loaded successfully: %s"),
			*LoadedMaterial->GetName());
		Debug::Print(Msg);
	}
	else
	{
		FString Msg = FString::Printf(TEXT("UMagicItemScanningSkill::Failed to load scan material from path: %s"),
			*MaterialPath);
		Debug::Print(Msg);
	}
}

void UMagicItemScanningSkill::LoadParameterCollection()
{
	FString Path = TEXT("/Game/Blueprints/Skills/SkillObject/ItemScanning/Materials/MPC_ScanPulse.MPC_ScanPulse");
	UMaterialParameterCollection* LoadedMPC = LoadObject<UMaterialParameterCollection>(nullptr, *Path);

	if (LoadedMPC)
	{
		ScanParameterCollection = LoadedMPC;
		Debug::Print(FString::Printf(TEXT("Loaded MPC: %s"), *LoadedMPC->GetName()));
	}
	else
	{
		Debug::Print(TEXT("Failed to load MPC_ScanPulse"));
	}
}

void UMagicItemScanningSkill::BackupOriginalBlendables()
{
	if (!PostProcessVolume)
	{
		Debug::PrintLog(TEXT("UMagicItemScanningSkill::Cannot backup - PostProcessVolume is null"));
		return;
	}

	// 원본 블렌더블 설정 백업
	OriginalBlendables = PostProcessVolume->Settings.WeightedBlendables.Array;
	bIsBackupValid = true;

	FString Msg = FString::Printf(TEXT("UMagicItemScanningSkill::Original blendables backed up - Count: %d"),
		OriginalBlendables.Num());
	Debug::Print(Msg);
}

void UMagicItemScanningSkill::SwapPostProcessMaterial()
{
	if (!PostProcessVolume || !ScanMaterial)
	{
		Debug::PrintLog(TEXT("UMagicItemScanningSkill::PostProcessVolume or ScanMaterial is null"));
		return;
	}

	FWeightedBlendable WeightedBlendable;
	WeightedBlendable.Object = ScanMaterial;
	WeightedBlendable.Weight = 1.0f;

	// 기존 블렌더블 제거
	PostProcessVolume->Settings.WeightedBlendables.Array.Empty();
	// 새 머티리얼 추가
	PostProcessVolume->Settings.WeightedBlendables.Array.Add(WeightedBlendable);

	Debug::PrintLog(TEXT("UMagicItemScanningSkill::PostProcess material swapped successfully"));
}

void UMagicItemScanningSkill::RestoreOriginalPostProcess()
{
	if (!PostProcessVolume || !bIsBackupValid)
	{
		Debug::PrintLog(TEXT("UMagicItemScanningSkill::Cannot restore - PostProcessVolume is null or backup is invalid"));
		return;
	}

	// 원본 블렌더블 설정 복원
	PostProcessVolume->Settings.WeightedBlendables.Array = OriginalBlendables;

	Debug::PrintLog(TEXT("UMagicItemScanningSkill::PostProcess settings restored to original state"));

	// 백업 상태 리셋
	bIsBackupValid = false;
	OriginalBlendables.Empty();
}

void UMagicItemScanningSkill::EndScannning()
{
	UKismetMaterialLibrary::SetScalarParameterValue(GetWorld(), ScanParameterCollection, FName("Radius"), 0.0f);

	IsStart = false;

	// 원본 PostProcess 설정 복원
	RestoreOriginalPostProcess();
}