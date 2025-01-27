#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"

#include "HealthInterface.generated.h"

class UHealthComponent;

UINTERFACE(MinimalAPI, Blueprintable)
class UHealthInterface : public UInterface
{
	GENERATED_BODY()
};

class ROBOTGAME_API IHealthInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Health)
	void CheckHealth(AActor* HealthKeeper, UHealthComponent* ParameterHealthComponent); // implemented in inherited classes
};
