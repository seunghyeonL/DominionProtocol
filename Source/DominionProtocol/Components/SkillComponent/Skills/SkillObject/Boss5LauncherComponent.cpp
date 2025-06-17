#include "Components/SkillComponent/Skills/SkillObject/Boss5LauncherComponent.h"
#include "Boss5LauncherComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Util/DebugHelper.h"

UBoss5LauncherComponent::UBoss5LauncherComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	ProjectileSpeed = 2000.0f;
	AttackCooldown = 1.0f;
	bCanLaunchProjectile = true; 
	LaunchSocketName = "None";
}

void UBoss5LauncherComponent::BeginPlay()
{
	Super::BeginPlay();

	if (!GetOwner()->FindComponentByClass<UStaticMeshComponent>())
	{
		
	}
}


void UBoss5LauncherComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UBoss5LauncherComponent::LaunchProjectileAtTarget(AActor* TargetActor)
{
	if (!bCanLaunchProjectile || !ProjectileClass || !TargetActor)
	{
		return;
	}

	UStaticMeshComponent* MeshComponent = GetOwner()->FindComponentByClass<UStaticMeshComponent>();
	FVector SpawnLocation = FVector::ZeroVector;
	FRotator SpawnRotation = FRotator::ZeroRotator;

	if (MeshComponent && MeshComponent->DoesSocketExist(LaunchSocketName))
	{
		SpawnLocation = MeshComponent->GetSocketLocation(LaunchSocketName);
		SpawnRotation = MeshComponent->GetSocketRotation(LaunchSocketName);
	}
	else
	{
		SpawnLocation = GetOwner()->GetActorLocation();
		SpawnRotation = GetOwner()->GetActorRotation();
	}

	FVector DirectionToTarget = (TargetActor->GetActorLocation() - SpawnLocation).GetSafeNormal();
	SpawnRotation = DirectionToTarget.Rotation();

	SpawnProjectile(SpawnLocation, SpawnRotation);
}

void UBoss5LauncherComponent::LaunchProjectileInDirection(const FVector& LaunchDirection)
{
	if (!bCanLaunchProjectile || !ProjectileClass)
	{
		return;
	}
	UStaticMeshComponent* MeshComponent = GetOwner()->FindComponentByClass<UStaticMeshComponent>();
	FVector SpawnLocation = FVector::ZeroVector;
	FRotator SpawnRotation = FRotator::ZeroRotator;

	if (MeshComponent && MeshComponent->DoesSocketExist(LaunchSocketName))
	{
		SpawnLocation = MeshComponent->GetSocketLocation(LaunchSocketName);
		SpawnRotation = LaunchDirection.Rotation();
	}
	else
	{
		SpawnLocation = GetOwner()->GetActorLocation();
		SpawnRotation = LaunchDirection.Rotation();
	}

	SpawnProjectile(SpawnLocation, SpawnRotation);
}

void UBoss5LauncherComponent::OnCooldownFinished()
{
	bCanLaunchProjectile = true;
}

void UBoss5LauncherComponent::SpawnProjectile(const FVector& SpawnLocation, const FRotator& SpawnRotation)
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = GetOwner(); // 발사한 액터를 Owner로 설정 (피해 계산 등)
	SpawnParams.Instigator = GetOwner()->GetInstigator(); // 발사한 인스티게이터 설정
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	AActor* NewProjectile = GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnLocation, SpawnRotation, SpawnParams);

	if (NewProjectile)
	{
		UProjectileMovementComponent* ProjectileMovement = NewProjectile->FindComponentByClass<UProjectileMovementComponent>();
		if (ProjectileMovement)
		{
			ProjectileMovement->SetVelocityInLocalSpace(FVector::ForwardVector * ProjectileSpeed);
		}

		// 쿨타임 시작
		bCanLaunchProjectile = false;
		GetWorld()->GetTimerManager().SetTimer(CooldownTimerHandle, this, &UBoss5LauncherComponent::OnCooldownFinished, AttackCooldown, false);
	}
}

