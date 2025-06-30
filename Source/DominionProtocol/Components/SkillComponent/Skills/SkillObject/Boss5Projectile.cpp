#include "Components/SkillComponent/Skills/SkillObject/Boss5Projectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "NiagaraComponent.h"   
#include "NiagaraFunctionLibrary.h"

ABoss5Projectile::ABoss5Projectile()
{
	PrimaryActorTick.bCanEverTick = false;

	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	RootComponent = CollisionComp;
	CollisionComp->InitSphereRadius(15.0f);
	CollisionComp->SetCollisionProfileName(TEXT("Projectile"));
	CollisionComp->OnComponentHit.AddDynamic(this, &ABoss5Projectile::OnHit);
	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &ABoss5Projectile::OnOverlapBegin);

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	ProjectileMesh->SetupAttachment(RootComponent);
	ProjectileMesh->SetCollisionProfileName(TEXT("NoCollision"));

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->SetUpdatedComponent(CollisionComp);
	ProjectileMovement->InitialSpeed = 0.0f;
	ProjectileMovement->MaxSpeed = 0.0f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;
	ProjectileMovement->ProjectileGravityScale = 0.0f;

	NiagaraEffectComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraEffectComp"));
	NiagaraEffectComp->SetupAttachment(RootComponent);
	NiagaraEffectComp->bAutoActivate = true; // 스폰될 때 자동으로 나이아가라 이펙트 재생

	DamageAmount = 100.0f;

}

void ABoss5Projectile::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABoss5Projectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABoss5Projectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor && (OtherActor != GetOwner()) && (OtherActor != this))
	{
		//Destroy();
	}
}

void ABoss5Projectile::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != GetOwner()) && (OtherActor != this))
	{
		//Destroy(); 
	}
}

