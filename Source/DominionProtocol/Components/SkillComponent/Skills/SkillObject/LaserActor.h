// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Util/GameTagList.h"
#include "LaserActor.generated.h"

class UNiagaraComponent;
class UMnhTracerComponent;
class UMnhCapsuleComponent;

UCLASS()
class DOMINIONPROTOCOL_API ALaserActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ALaserActor();

	virtual void SetOwnerCharacter(AActor* NewOwnerCharacter);

	// 히트 감지 시 호출될 함수
	UFUNCTION()
	void HandleTracerHit(FGameplayTag TracerTag, FHitResult HitResult, float DeltaTime);

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Initialize();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill")
	TObjectPtr<AActor> OwnerCharacter;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill")
	TObjectPtr<USceneComponent> SceneComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill")
	TObjectPtr<UNiagaraComponent> LaserBeamEffect;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill")
	TObjectPtr<UNiagaraComponent> LaserFireBallEffect;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill")
	TObjectPtr<UNiagaraComponent> LaserHitEffect;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill")
	TObjectPtr<UMaterialInterface> DecalMaterial1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill")
	TObjectPtr<UMaterialInterface> DecalMaterial2;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill")
	TObjectPtr<UMnhCapsuleComponent> CapsuleComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill")
	FGameplayTag OwnerSkillTag;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill")
	TObjectPtr<UMnhTracerComponent> TraceComponent;

	float TraceLength;
};
