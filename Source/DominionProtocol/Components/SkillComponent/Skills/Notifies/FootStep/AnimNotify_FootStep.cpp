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
	
						// 바닥 라인트레이스
						FHitResult HitResult;
						FVector Start = OwnerCharacter->GetActorLocation();
						FVector End = Start - FVector(0, 0, 100.f); // 적당한 거리
						
						FCollisionQueryParams Params;
						Params.AddIgnoredActor(OwnerCharacter);
						Params.bReturnPhysicalMaterial = true;
						
						if (OwnerCharacter->GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, Params))
						{
							if (UPhysicalMaterial* PhysMat = HitResult.PhysMaterial.Get())
							{
								SurfaceType = PhysMat->SurfaceType;
							}
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
								if (IsValid(SurfaceTypeData->FootStepSound))
								{
									UGameplayStatics::PlaySoundAtLocation(MeshComp, SurfaceTypeData->FootStepSound, FootStepLocation);
								}
						
								// FootStepVfx 실행
								if (IsValid(SurfaceTypeData->FootStepVfx))
								{
									UNiagaraFunctionLibrary::SpawnSystemAtLocation(
										MeshComp,
										SurfaceTypeData->FootStepVfx,
										FootStepLocation,
										FootStepRotation,
										FVector(1.f),
										true,
										true
									);
								}
							}
						}
					}
				}
			}
		}
	}
}
