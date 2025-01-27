#include "RobotGame/public/Other/Projectile.h"

#include "Engine/DamageEvents.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

#include "RobotGame/public/Gameplay/Pawn/RobotPawn.h"
#include "RobotGame/public/Gameplay/Pawn/TurretPawn.h"


AProjectile::AProjectile(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;

	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Collision sphere"));
	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile"));
	ProjectileMovementComponent =
		CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile movement component"));

	SetRootComponent(CollisionSphere);
	ProjectileMesh->SetupAttachment(RootComponent);

	ProjectileMovementComponent->InitialSpeed = 4000.f;
	ProjectileMovementComponent->MaxSpeed = 4000.f;
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->bShouldBounce = false;
	ProjectileMovementComponent->ProjectileGravityScale = 0.1f;

	Damage = 10.f;
}

void AProjectile::FireInDirection(const FVector& ShootDirection)
{
	if (ProjectileMovementComponent->IsValidLowLevel())
	{
		ProjectileMovementComponent->Velocity = ShootDirection * ProjectileMovementComponent->InitialSpeed;
	}
}

void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	CollisionSphere->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);

	if (SpawnEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SpawnEffect, GetActorLocation());
	}
}

void AProjectile::OnHit(
	UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor && OtherActor != GetOwner() && OtherActor != GetInstigator())
	{
		if (DamageEffect)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DamageEffect, Hit.Location);
		}

		if (GetInstigator() &&
			GetInstigator()->GetClass()->ImplementsInterface(UShootingInterface::StaticClass())
			&& OtherActor->GetClass()->ImplementsInterface(UShootingInterface::StaticClass()))
		{
			OtherActor->TakeDamage(Damage, {}, nullptr, this);

			UKismetSystemLibrary::PrintString(GetWorld(), "Pawn have gotten damage", true, false, FColor::Red, 1.f);
		}

		this->Destroy();
	}
}
