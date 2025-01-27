#pragma once

#include "CoreMinimal.h"
#include "RobotGame/public/Other/SpawnerItems.h"

#include "OverchargeItem.generated.h"

UCLASS()
class ROBOTGAME_API AOverchargeItem : public ASpawnerItems
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Overcharge")
	float BonusEnergy;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Overcharge")
	//float TimeOverChargeEnd;

public:
	AOverchargeItem(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	virtual void SpecialBonus(AActor* Actor) override;
};
