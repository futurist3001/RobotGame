#include "RobotGame/public/Gameplay/Core/RobotPlayerController.h"

#include "RobotGame/public/Gameplay/Pawn/RobotPawn.h"
#include "RobotGame/public/Gameplay/UI/RobotHUDUserWidget.h"
#include "RobotGame/public/Other/HealthComponent.h"

void ARobotPlayerController::BeginPlay()
{
	Super::BeginPlay();

	CreateRobotHUDUserWidget();
}

void ARobotPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (RobotHUDUserWidget)
	{
		UpdateHUDWidget();
	}
}

void ARobotPlayerController::CreateRobotHUDUserWidget()
{
	if (RobotHUDUserWidgetClass)
	{
		RobotHUDUserWidget = CreateWidget<URobotHUDUserWidget>(this, RobotHUDUserWidgetClass);
		RobotHUDUserWidget->AddToViewport();
		RobotHUDUserWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void ARobotPlayerController::UpdateHUDWidget()
{
	if (ARobotPawn* RobotPawn = Cast<ARobotPawn>(GetPawn()))
	{
		if (RobotHUDUserWidget)
		{
			RobotHUDUserWidget->SetHealth(
				RobotPawn->HealthComponent->Health, RobotPawn->HealthComponent->DefaultHealth);

			RobotHUDUserWidget->SetEnergy(
				RobotPawn->GetCuurentEnergy(), RobotPawn->DefaultEnergy);
		}
	}
	
}
