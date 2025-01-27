#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"

#include "RobotGame/public/Generic/Interfaces/ShootingInterface.h"
#include "RobotGame/public/Generic/Interfaces/HealthInterface.h"

#include "TurretPawn.generated.h"

class AProjectile;
class UCapsuleComponent;
class UHealthComponent;
class USphereComponent;

UCLASS()
class ROBOTGAME_API ATurretPawn : public APawn, public IShootingInterface, public IHealthInterface
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, Category = "Collision")
	TObjectPtr<UCapsuleComponent> CollisionCapsule;

	UPROPERTY(EditAnywhere, Category = "Shooting")
	TObjectPtr<USphereComponent> ShootingZone;

	UPROPERTY(EditAnywhere, Category = "SM")
	TObjectPtr<UStaticMeshComponent> BaseBaseMesh;

	UPROPERTY(EditAnywhere, Category = "SM")
	TObjectPtr<UStaticMeshComponent> BaseMesh;

	UPROPERTY(EditAnywhere, Category = "SM")
	TObjectPtr<UStaticMeshComponent> TurretMesh;

	UPROPERTY(EditAnywhere, Category = "ProjectileSpawnLocation")
	TObjectPtr<USceneComponent> ProjectileSpawnLocationFirst;

	UPROPERTY(EditAnywhere, Category = "ProjectileSpawnLocation")
	TObjectPtr<USceneComponent> ProjectileSpawnLocationSecond;

	UPROPERTY(EditAnywhere, Category = "ProjectileSpawnLocation")
	TObjectPtr<USceneComponent> ProjectileSpawnLocationThird;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rotation")
	float RotationSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shooting")
	float ShootingTime;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health")
	TObjectPtr<UHealthComponent> HealthComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "VFX")
	TObjectPtr<UParticleSystem> DeathEffect;

protected:
	UPROPERTY(Transient, BlueprintReadOnly)
	TArray<AActor*> Offenders;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile")
	TSubclassOf<AProjectile> ProjectileClass;

	//uint8 bIsLookToRobot : 1;

private:
	float DT; // value of Delta Time from Tick()

	FTimerHandle ActiveteShootingTimer;

public:
	ATurretPawn(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	virtual void Shooting_Implementation() override;
	void ShootLogic(const FVector& Start);

	UFUNCTION()
	void InitCheckHelth(AActor* HealthKeeper, UHealthComponent* ParameterHealthComponent);
	virtual void CheckHealth_Implementation(AActor* HealthKeeper, UHealthComponent* ParameterHealthComponent) override;

	void RotateTurret();
	bool FindIsTurretLookToRobot();

private:
	UFUNCTION()
	void OnBeginOverlap(
		UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnEndOverlap(
		UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);
};
