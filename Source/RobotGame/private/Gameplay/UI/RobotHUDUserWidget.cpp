#include "RobotGame/public/Gameplay/UI/RobotHUDUserWidget.h"

#include "Components/ProgressBar.h"

void URobotHUDUserWidget::SetHealth(float CurrentHealth, float DefaultHealth)
{
	if (HealthBar)
	{
		HealthBar->SetPercent(CurrentHealth / DefaultHealth);
	}
}

void URobotHUDUserWidget::SetEnergy(float CurrentEnergy, float DefaultEnergy)
{
	if (EnergyBar)
	{
		EnergyBar->SetPercent(CurrentEnergy / DefaultEnergy);
	}
}
