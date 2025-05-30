#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "WeaponData.generated.h"

USTRUCT(BlueprintType)
struct FWeaponData : public FTableRowBase
{
	GENERATED_BODY()

public:
	//장착 소켓 이름
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Data")
	FName SocketName;

	//무기 메시
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Data")
	TObjectPtr<USkeletalMeshComponent> WeaponMesh;

	//트레이스박스 위치
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Data")
	FTransform TraceBoxTransform;
};