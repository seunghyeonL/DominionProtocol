// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_FootStep.h"

#include "NiagaraFunctionLibrary.h"
#include "DomiFramework/GameState/BaseGameState.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"
#include "EnumAndStruct/PhysicalSurfaceTypeData/PhysicalSurfaceTypeData.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Util/AsyncLoadBPLib.h"
#include "Util/DebugHelper.h"

void UAnimNotify_FootStep::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                  const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (IsValid(MeshComp))
	{
		if (AActor* Owner = MeshComp->GetOwner())
		{
			if (ACharacter* OwnerCharacter = Cast<ACharacter>(Owner))
			{
				auto MovementComponent = OwnerCharacter->GetCharacterMovement();
				if (MovementComponent && MovementComponent->IsMovingOnGround())
				{
					const FFindFloorResult& CurrentFloor = MovementComponent->CurrentFloor;
					if (CurrentFloor.IsWalkableFloor())
					{
						// PhysicalMaterial 설정되어있으면 그거에 맞게, 아니면 Default로.
						EPhysicalSurface SurfaceType = SurfaceType_Default;
						if (UPhysicalMaterial* PhysMat = CurrentFloor.HitResult.PhysMaterial.Get()) 
						{
							SurfaceType = PhysMat->SurfaceType;
						}
						
						if (auto GS = Cast<ABaseGameState>(MeshComp->GetWorld()->GetGameState()))
						{
							if (FPhysicalSurfaceTypeData* SurfaceTypeData = GS->GetPhysicalSurfaceTypeData(SurfaceType))
							{
								check(MeshComp->DoesSocketExist(SocketName));
								
								FStreamableManager& Streamable = UAssetManager::GetStreamableManager();
								FVector FootStepLocation = MeshComp->GetSocketLocation(SocketName);
								FRotator FootStepRotation = MeshComp->GetSocketRotation(SocketName);
								
								// FootStep사운드 실행
								UAsyncLoadBPLib::AsyncPlaySoundAtLocation(MeshComp, SurfaceTypeData->FootStepSound, FootStepLocation);
						
								// FootStepVfx 실행
								UAsyncLoadBPLib::AsyncSpawnNiagaraSystem(MeshComp, SurfaceTypeData->FootStepVfx, FootStepLocation, FootStepRotation);
							}
						}
					}
				}
			}
		}
	}
}
