#include "Components/SkillComponent/Skills/SkillObject/Boss5LauncherComponent.h"
#include "Boss5LauncherComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Engine/World.h"
#include "Util/DebugHelper.h"

UBoss5LauncherComponent::UBoss5LauncherComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	ProjectileSpeed = 500.0f;
	AttackCooldown = 1.0f;
	bCanLaunchProjectile = true; 
	LaunchSocketName = "None";
	CurrentTargetActor = nullptr;
}

void UBoss5LauncherComponent::BeginPlay()
{
	Super::BeginPlay();
}


void UBoss5LauncherComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UBoss5LauncherComponent::SetAndTrackTarget(AActor* NewTargetActor)
{
}

void UBoss5LauncherComponent::ClearTarget()
{
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

}

void UBoss5LauncherComponent::LaunchProjectileInDirection(const FVector& LaunchDirection)
{
	if (!bCanLaunchProjectile || !ProjectileClass)
	{
		return;
	}
	if (LaunchDirection.IsNearlyZero())
	{
		Debug::Print(TEXT("LaunchProjectileInDirection: LaunchDirection is nearly zero. Cannot launch projectile."));
		return;
	}

	UStaticMeshComponent* MeshComponent = GetOwner()->FindComponentByClass<UStaticMeshComponent>();
	FVector SpawnLocation = FVector::ZeroVector;
	FRotator SpawnRotation = FRotator::ZeroRotator; // 실제 이동 방향과는 별개로 프로젝타일 메시의 초기 회전

	if (MeshComponent && MeshComponent->DoesSocketExist(LaunchSocketName))
	{
		SpawnLocation = MeshComponent->GetSocketLocation(LaunchSocketName);
		// 소켓의 회전을 사용하는 대신, 발사 방향을 기준으로 프로젝타일의 회전을 설정
		SpawnRotation = LaunchDirection.Rotation();
	}
	else
	{
		SpawnLocation = GetOwner()->GetActorLocation();
		// 소켓이 없을 경우, 발사하는 액터의 위치에서 발사 방향을 기준으로 회전을 설정
		SpawnRotation = LaunchDirection.Rotation();
	}

	SpawnProjectile(SpawnLocation, SpawnRotation, LaunchDirection.GetSafeNormal()); // 방향을 전달
}

void UBoss5LauncherComponent::OnCooldownFinished()
{
	bCanLaunchProjectile = true;
}

void UBoss5LauncherComponent::SpawnProjectile(const FVector& SpawnLocation, const FRotator& SpawnRotation, const FVector& LaunchDirection)
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}
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
			ProjectileMovement->InitialSpeed = ProjectileSpeed; // 초기 속도 설정
			ProjectileMovement->MaxSpeed = ProjectileSpeed;     // 최대 속도 설정 (옵션)
			ProjectileMovement->Velocity = LaunchDirection.GetSafeNormal() * ProjectileSpeed; // 원하는 방향 * 속도
		}

		// 쿨타임 시작
		bCanLaunchProjectile = false;
		World->GetTimerManager().SetTimer(CooldownTimerHandle, this, &UBoss5LauncherComponent::OnCooldownFinished, AttackCooldown, false);
	}
}

