#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "RobotHUDUserWidget.generated.h"

class UProgressBar;

UCLASS()
class ROBOTGAME_API URobotHUDUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> HealthBar;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> EnergyBar;

public:
	void SetHealth(float CurrentHealth, float DefaultHealth);
	void SetEnergy(float CurrentEnergy, float DefaultEnergy);
};
