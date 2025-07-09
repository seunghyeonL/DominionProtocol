#pragma once
#include "CoreMinimal.h"
#include "Components/SkillComponent/Skills/BaseSkill.h"
#include "MagicItemScanningSkill.generated.h"

class APostProcessVolume;
class UMaterialInterface;
class UMaterialParameterCollection;

UCLASS()
class DOMINIONPROTOCOL_API UMagicItemScanningSkill : public UBaseSkill
{
    GENERATED_BODY()

public:
    UMagicItemScanningSkill();

    virtual void Execute() override;

    void EndScannning();

    UPROPERTY(EditDefaultsOnly, Category = "Scan")
    TObjectPtr<UMaterialParameterCollection> ScanParameterCollection;

    UPROPERTY(BlueprintReadWrite, Category = "Scan")
    bool IsStart;

private:

    // 머티리얼 교체 함수
    UFUNCTION()
    void SwapPostProcessMaterial();

    // 원본 PostProcess 설정 복원 함수
    UFUNCTION()
    void RestoreOriginalPostProcess();

    // 특정 PostProcessVolume을 찾는 함수
    UFUNCTION()
    void FindAndSetPostProcessVolume();

    // 머티리얼을 로드하는 함수
    UFUNCTION()
    void LoadScanMaterial();

    // MPC 로드하는 함수
    UFUNCTION()
    void LoadParameterCollection();

    // 원본 블렌더블 설정을 백업하는 함수
    UFUNCTION()
    void BackupOriginalBlendables();

    UPROPERTY()
    TObjectPtr<APostProcessVolume> PostProcessVolume;

    // 교체할 머티리얼 인터페이스
    UPROPERTY()
    TObjectPtr<UMaterialInterface> ScanMaterial;

    // 원본 블렌더블 설정 백업
    UPROPERTY()
    TArray<FWeightedBlendable> OriginalBlendables;

    // 백업이 되었는지 확인하는 플래그
    UPROPERTY()
    bool bIsBackupValid;

    FTimerHandle ScanningTimerHandle;

    float Duration;
};