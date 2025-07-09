#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "AccessoryData.generated.h"

USTRUCT(BlueprintType)
struct FAccessoryData : public FTableRowBase
{
	GENERATED_BODY()

public:
	//장착 소켓 이름
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Data")
	FName SocketName;

	//악세서리 메시
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Data")
	TObjectPtr<USkeletalMeshComponent> AccessoryMesh;

	
};