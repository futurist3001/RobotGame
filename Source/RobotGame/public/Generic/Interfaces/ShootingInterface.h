#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"

#include "ShootingInterface.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UShootingInterface : public UInterface
{
	GENERATED_BODY()
};


class ROBOTGAME_API IShootingInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Shooting)
	void Shooting(); // implemented in inherited classes
};
