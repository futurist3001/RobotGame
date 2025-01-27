#pragma once

#include "CoreMinimal.h"
#include "RobotGame/public/Other/SpawnerItems.h"

#include "HealthItem.generated.h"


UCLASS()
class ROBOTGAME_API AHealthItem : public ASpawnerItems
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float BonusHealth;

public:
	AHealthItem(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	virtual void SpecialBonus(AActor* Actor) override;
	
};
