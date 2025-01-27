#include "RobotGame/public/Gameplay/Pawn/TurretPawn.h"

#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"

#include "RobotGame/public/Other/Projectile.h"
#include "RobotGame/public/Gameplay/Pawn/RobotPawn.h"
#include "RobotGame/public/Other/HealthComponent.h"

ATurretPawn::ATurretPawn(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;

	CollisionCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Collision capsule"));
	ShootingZone = CreateDefaultSubobject<USphereComponent>(TEXT("Shooting zone"));
	BaseBaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base mesh"));
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base base mesh"));
	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret mesh"));
	ProjectileSpawnLocationFirst = CreateDefaultSubobject<USceneComponent>(TEXT("First projectile spawn location"));
	ProjectileSpawnLocationSecond = CreateDefaultSubobject<USceneComponent>(TEXT("Second projectile spawn location"));
	ProjectileSpawnLocationThird = CreateDefaultSubobject<USceneComponent>(TEXT("Third projectile spawn location"));
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health component"));

	SetRootComponent(CollisionCapsule);
	ShootingZone->SetupAttachment(RootComponent);
	BaseBaseMesh->SetupAttachment(RootComponent);
	BaseMesh->SetupAttachment(BaseBaseMesh);
	TurretMesh->SetupAttachment(BaseMesh);
	ProjectileSpawnLocationFirst->SetupAttachment(TurretMesh);
	ProjectileSpawnLocationSecond->SetupAttachment(TurretMesh);
	ProjectileSpawnLocationThird->SetupAttachment(TurretMesh);

	RotationSpeed = 2.f;
	ShootingTime = 3.f;

}

void ATurretPawn::BeginPlay()
{
	Super::BeginPlay();
	
	if (HealthComponent)
	{
		HealthComponent->HealthChanged.AddDynamic(this, &ATurretPawn::InitCheckHelth);
	}

	ShootingZone->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnBeginOverlap);
	ShootingZone->OnComponentEndOverlap.AddDynamic(this, &ThisClass::OnEndOverlap);
}

void ATurretPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	DT = DeltaTime;

	if (!Offenders.IsEmpty())
	{
		RotateTurret();
	}
}

void ATurretPawn::Shooting_Implementation()
{
	if (FindIsTurretLookToRobot())
	{
		FTimerHandle FirstShootTimer;
		FTimerHandle SecondShootTimer;
		FTimerHandle ThirdShootTimer;

		GetWorld()->GetTimerManager().SetTimer(
			FirstShootTimer, [this]()
			{
				ShootLogic(ProjectileSpawnLocationFirst->GetComponentLocation());
			}, 0.1f, false);

		GetWorld()->GetTimerManager().SetTimer(
			SecondShootTimer, [this]()
			{
				ShootLogic(ProjectileSpawnLocationSecond->GetComponentLocation());
			}, 1.1f, false);

		GetWorld()->GetTimerManager().SetTimer(
			ThirdShootTimer, [this]()
			{
				ShootLogic(ProjectileSpawnLocationThird->GetComponentLocation());
			}, 2.1f, false);
	}
}

void ATurretPawn::ShootLogic(const FVector& Start)
{
	if (!Offenders.IsEmpty() && Offenders[0]->IsValidLowLevel())
	{
		FVector End = Offenders[0]->GetActorLocation();
		FVector ShootDirection = (End - Start).GetSafeNormal();

		FActorSpawnParameters SpawnParameters;
		SpawnParameters.Owner = SpawnParameters.Instigator = this;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(
			ProjectileClass, Start, ShootDirection.Rotation(), SpawnParameters);
		Projectile->FireInDirection(ShootDirection);
	}
}

void ATurretPawn::CheckHealth_Implementation(AActor* HealthKeeper, UHealthComponent* ParameterHealthComponent)
{
	if (IsValid(HealthKeeper))
	{
		UKismetSystemLibrary::PrintString(
			this, FString::SanitizeFloat(ParameterHealthComponent->Health), true, false, FColor::Green, 2.f);

		if (ParameterHealthComponent->Health <= 0)
		{
			if (DeathEffect)
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DeathEffect, GetActorLocation());
			}

			HealthKeeper->Destroy();
		}
	}
}

void ATurretPawn::InitCheckHelth(AActor* HealthKeeper, UHealthComponent* ParameterHealthComponent)
{
	Execute_CheckHealth(this, HealthKeeper, ParameterHealthComponent);
}

void ATurretPawn::RotateTurret()
{
	if (!Offenders.IsEmpty())
	{
		FRotator TargetRotator = UKismetMathLibrary::FindLookAtRotation(
			GetActorLocation(), Offenders[0]->GetActorLocation());

		BaseMesh->SetWorldRotation(FMath::RInterpTo(
			TurretMesh->GetComponentRotation(),
			TargetRotator,
			DT,
			RotationSpeed));
	}
}

bool ATurretPawn::FindIsTurretLookToRobot()
{
	FVector Start = ProjectileSpawnLocationSecond->GetComponentLocation();
	FVector End = Start + ProjectileSpawnLocationSecond->GetForwardVector() * 5000.f;

	if (Offenders[0] && Offenders[0]->IsValidLowLevel())
	{
		Start.Z = Offenders[0]->GetActorLocation().Z;
		End.Z = Offenders[0]->GetActorLocation().Z;
	}

	FHitResult HitResult;

	if (!Offenders.IsEmpty())
	{
		if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECollisionChannel::ECC_Visibility))
		{
			//DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 5.0f, 0, 3.0f);

			if (HitResult.GetActor()->IsA<ARobotPawn>())
			{
				UKismetSystemLibrary::PrintString(GetWorld(), "Finded target", true, false, FColor::Black, 3.f);

				if (ARobotPawn* RobotPawn = Cast<ARobotPawn>(HitResult.GetActor()))
				{
					if (RobotPawn->GetActorLocation() == Offenders[0]->GetActorLocation())
					{
						return true;
					}
				}
			}
		}
	}

	return false;
}

void ATurretPawn::OnBeginOverlap(
	UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor->IsValidLowLevel() && OtherActor->IsA<ARobotPawn>())
	{
		//UKismetSystemLibrary::PrintString(GetWorld(), "Turret is activated", true, false, FColor::Cyan, 5.f);

		Offenders.Add(OtherActor);

		GetWorld()->GetTimerManager().SetTimer(
			ActiveteShootingTimer, [this]()
			{
				Execute_Shooting(this);
			}, ShootingTime, true);
	}
}

void ATurretPawn::OnEndOverlap(
	UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && OtherActor->IsValidLowLevel() && OtherActor->IsA<ARobotPawn>())
	{
		//UKismetSystemLibrary::PrintString(GetWorld(), "Turret is deactivated", true, false, FColor::Cyan, 5.f);

		Offenders.RemoveSingleSwap(OtherActor);

		if (Offenders.IsEmpty())
		{
			GetWorldTimerManager().ClearTimer(ActiveteShootingTimer); // deactivate ActiveteShootingTimer
		}
	}
}

