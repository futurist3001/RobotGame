#pragma once

#include "CoreMinimal.h"
#include "RobotGame/public/Other/SpawnerItems.h"

#include "RushItem.generated.h"


UCLASS()
class ROBOTGAME_API ARushItem : public ASpawnerItems
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Speed")
	float BonusSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Speed")
	float TimeResetSpeed;

private:

public:
	ARushItem(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	virtual void SpecialBonus(AActor* Actor) override;
	
};
