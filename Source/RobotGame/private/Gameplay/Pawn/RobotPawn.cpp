#include "RobotGame/public/Gameplay/Pawn/RobotPawn.h"

#include "Camera/CameraComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Components/InputComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"

#include "RobotGame/public/Other/Projectile.h"
#include "RobotGame/public/Other/HealthComponent.h"

ARobotPawn::ARobotPawn(const FObjectInitializer& ObjectInitializer)
: Super (ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;

	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere collision component"));
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Robot mesh"));
	ProjectileSpawnLocation = CreateDefaultSubobject<USceneComponent>(TEXT("Projectile spawn location"));
	FirstPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("First person camera"));
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring arm component"));
	ThirdPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Third person camera"));
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health component"));
	MoveForwardBackwardPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Move forward backward point"));

	SetRootComponent(SphereCollision);
	StaticMesh->SetupAttachment(RootComponent);
	ProjectileSpawnLocation->SetupAttachment(RootComponent);
	FirstPersonCamera->SetupAttachment(RootComponent);
	SpringArmComponent->SetupAttachment(RootComponent);
	ThirdPersonCamera->SetupAttachment(SpringArmComponent);
	MoveForwardBackwardPoint->SetupAttachment(StaticMesh);

	Speed = 10.f;

	RechargeTime = CurrentTimeFire = 0.75f;

	DefaultEnergy = CurrentEnergy = 50.f;
	EnergyRecoverTime = 50.f;
	CurrentTimeEnergy = 0.0f;
}

void ARobotPawn::BeginPlay()
{
	Super::BeginPlay();

	if (HealthComponent)
	{
		HealthComponent->HealthChanged.AddDynamic(this, &ARobotPawn::InitCheckHelth);
	}

	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (auto Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(RobotMapingContext, 0);
		}
	}
}

void ARobotPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CurrentTimeFire += DeltaTime;
	CurrentTimeEnergy += DeltaTime;

	if (CurrentEnergy < DefaultEnergy)
	{
		if (CurrentTimeEnergy >= EnergyRecoverTime)
		{
			CurrentEnergy += 10.f;
			CurrentTimeEnergy = 0.0f;
		}
	}
	
	//UKismetSystemLibrary::PrintString(GetWorld(), GetActorRotation().ToString(), true, false, FColor::Blue, DeltaTime);
	//UKismetSystemLibrary::PrintString(GetWorld(), GetActorLocation().ToString(), true, false, FColor::Blue, DeltaTime);
}

void ARobotPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	if (auto EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveBackwardForwardAction, ETriggerEvent::Triggered, this, &ARobotPawn::MoveBackwardForward);
		EnhancedInputComponent->BindAction(MoveLeftRightAction, ETriggerEvent::Triggered, this, &ARobotPawn::MoveLeftRight);
		EnhancedInputComponent->BindAction(MoveUpDownAction, ETriggerEvent::Triggered, this, &ARobotPawn::MoveUptDown);
		EnhancedInputComponent->BindAction(RotateAction, ETriggerEvent::Triggered, this, &ARobotPawn::Rotate);
		EnhancedInputComponent->BindAction(ShootingAction, ETriggerEvent::Started, this, &ARobotPawn::Shoot);
	}
}

void ARobotPawn::MoveBackwardForward(const FInputActionValue& Value)
{
	FVector ForwBackwVec = Value.Get<FVector>();

	if (ForwBackwVec != FVector::ZeroVector)
	{
		//UKismetSystemLibrary::PrintString(GetWorld(), "Move forward/backward triggered", true, false, FColor::Green, 1.f);
		AddActorLocalOffset(ForwBackwVec * Speed, true, nullptr);

		if (Speed > 10.f) // more that default value (after some bonuses)
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(
				GetWorld(), MoveForwardBackwardEffect, MoveForwardBackwardPoint->GetComponentLocation());
		}
	}
}

void ARobotPawn::MoveLeftRight(const FInputActionValue& Value)
{
	FVector LeftRightVec = FVector(0.f, Value.Get<float>(), 0.f);

	if (LeftRightVec != FVector::ZeroVector)
	{
		//UKismetSystemLibrary::PrintString(GetWorld(), "Move left/right triggered", true, false, FColor::Blue, 1.f);
		AddActorLocalOffset(LeftRightVec * Speed, true, nullptr);
	}
}

void ARobotPawn::MoveUptDown(const FInputActionValue& Value)
{
	FVector UpDownVec = FVector(0.f, 0.f, Value.Get<float>());

	if (UpDownVec != FVector::ZeroVector)
	{
		//UKismetSystemLibrary::PrintString(GetWorld(), "Move up/down triggered", true, false, FColor::Purple , 1.f);
		AddActorLocalOffset(UpDownVec * Speed, true, nullptr);
	}
}

void ARobotPawn::Rotate(const FInputActionValue& Value)
{
	const FVector2D LookAxisValue = Value.Get<FVector2D>();

	if (GetController())
	{
		//UKismetSystemLibrary::PrintString(GetWorld(), "Rotate triggered", true, false, FColor::Cyan, 1.f);

		SetActorRotation(GetActorRotation() + FRotator(LookAxisValue.Y, 0.f, 0.f));

		if (abs((GetActorRotation().Pitch + LookAxisValue.X) - 90.f) < 1.f)
		{
			SetActorRotation(FRotator(89.f, GetActorRotation().Yaw, 0.f));
		}

		else if (abs((GetActorRotation().Pitch + LookAxisValue.X) + 90.f) < 1.f)
		{
			SetActorRotation(FRotator(-89.f, GetActorRotation().Yaw, 0.f));
		}
		
		else
		{
			SetActorRotation(GetActorRotation() + FRotator(0.f, LookAxisValue.X, 0.f));
		}
	}
}

void ARobotPawn::Shoot(const FInputActionValue& Value)
{
	bool IsShooting = Value.Get<bool>();

	if (IsShooting)
	{
		Execute_Shooting(this);
	}
}

void ARobotPawn::Shooting_Implementation()
{
	if (CurrentTimeFire >= RechargeTime && CurrentEnergy >= 10.f)
	{
		FVector Start = ProjectileSpawnLocation->GetComponentLocation();
		FVector End = Start + GetActorForwardVector() * 1000.f;
		FVector ShootDirection = (End - Start).GetSafeNormal();

		FActorSpawnParameters SpawnParameters;
		SpawnParameters.Owner = SpawnParameters.Instigator = this;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(
			ProjectileClass, Start, ShootDirection.Rotation(), SpawnParameters);
		Projectile->FireInDirection(ShootDirection);

		CurrentTimeFire = 0.0f;
		CurrentEnergy -= 10.f;
	}
}

void ARobotPawn::InitCheckHelth(AActor* HealthKeeper, UHealthComponent* ParameterHealthComponent)
{
	Execute_CheckHealth(this, HealthKeeper, ParameterHealthComponent);
}

void ARobotPawn::CheckHealth_Implementation(AActor* HealthKeeper, UHealthComponent* ParameterHealthComponent)
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

			FTimerHandle DeathTimer;
			GetWorldTimerManager().SetTimer(
				DeathTimer, [HealthKeeper]()
				{
					HealthKeeper->Destroy();
				}, 0.1f, false); // delay for updating hud widget in time
		}
	}
}

