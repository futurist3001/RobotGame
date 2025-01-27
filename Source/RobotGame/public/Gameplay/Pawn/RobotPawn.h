#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "InputActionValue.h"

#include "RobotGame/public/Generic/Interfaces/ShootingInterface.h"
#include "RobotGame/public/Generic/Interfaces/HealthInterface.h"

#include "RobotPawn.generated.h"

class AProjectile;
class UCameraComponent;
class UHealthComponent;
class USphereComponent;
class UInputMappingContext;
class UInputAction;

UCLASS()
class ROBOTGAME_API ARobotPawn : public APawn, public IShootingInterface, public IHealthInterface
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, Category = "Collision")
	TObjectPtr<USphereComponent> SphereCollision;

	UPROPERTY(VisibleAnywhere, Category = "SM")
	TObjectPtr<UStaticMeshComponent> StaticMesh;

	UPROPERTY(VisibleAnywhere, Category = "ProjectileSpawnLocation")
	TObjectPtr<USceneComponent> ProjectileSpawnLocation;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	TObjectPtr<UCameraComponent> Camera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shooting")
	float RechargeTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shooting")
	float DefaultEnergy;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shooting")
	float EnergyRecoverTime;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health")
	TObjectPtr<UHealthComponent> HealthComponent;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> RobotMapingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> MoveBackwardForwardAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> MoveLeftRightAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> MoveUpDownAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> RotateAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> ShootingAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile")
	TSubclassOf<AProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "VFX")
	TObjectPtr<UParticleSystem> DeathEffect;

private:
	float CurrentTimeFire;
	float CurrentTimeEnergy;

	float CurrentEnergy;

public:
	ARobotPawn(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	FORCEINLINE const float GetCurrentEnergy() const { return CurrentEnergy; }
	void SetCurrentEnergy(float NewEnergy) { CurrentEnergy = NewEnergy; }

protected:
	virtual void BeginPlay() override;	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void MoveBackwardForward(const FInputActionValue& Value);
	void MoveLeftRight(const FInputActionValue& Value);
	void MoveUptDown(const FInputActionValue& Value);
	void Rotate(const FInputActionValue& Value);

	void Shoot(const FInputActionValue& Value);
	virtual void Shooting_Implementation() override;

	UFUNCTION()
	void InitCheckHelth(AActor* HealthKeeper, UHealthComponent* ParameterHealthComponent);
	virtual void CheckHealth_Implementation(AActor* HealthKeeper, UHealthComponent* ParameterHealthComponent) override;

};
