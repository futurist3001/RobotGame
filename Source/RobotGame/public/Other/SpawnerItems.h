#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "SpawnerItems.generated.h"

class USphereComponent;
class UNiagaraComponent;
class UNiagaraSystem;

UCLASS()
class ROBOTGAME_API ASpawnerItems : public AActor
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category = "Collision")
	TObjectPtr<USphereComponent> SphereComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Niagara System")
	TObjectPtr<UNiagaraComponent> ItemVFX;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Niagara System")
	TObjectPtr<UNiagaraSystem> DeathVFX;

	UPROPERTY(EditAnywhere, Category = "Spawning")
	float RespawnTime;

	FTimerHandle RespawnTimerHandle;

private:

public:	
	ASpawnerItems(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	void Respawn();

protected:
	virtual void BeginPlay() override;
	virtual void SpecialBonus(AActor* Actor);

	UFUNCTION()
	virtual void OnBeginOverlap(
		UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
