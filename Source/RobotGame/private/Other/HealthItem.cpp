#include "RobotGame/public/Other/HealthItem.h"

#include "RobotGame/public/Gameplay/Pawn/RobotPawn.h"
#include "RobotGame/public/Other/HealthComponent.h"
#include "RobotGame/public/Gameplay/Core/RobotPlayerController.h"
#include "RobotGame/public/Gameplay/UI/RobotHUDUserWidget.h"

AHealthItem::AHealthItem(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;
}

void AHealthItem::SpecialBonus(AActor* Actor)
{
	if (Actor->IsValidLowLevel(); ARobotPawn* RobotPawn = Cast<ARobotPawn>(Actor))
	{
		if (RobotPawn->HealthComponent->Health + BonusHealth > RobotPawn->HealthComponent->DefaultHealth)
		{
			RobotPawn->HealthComponent->Health = 100.f;
		}
		
		else
		{
			RobotPawn->HealthComponent->Health += BonusHealth;
		}

		if (ARobotPlayerController* PlayerController = Cast<ARobotPlayerController>(RobotPawn->GetController()))
		{
			if (PlayerController->RobotHUDUserWidget)
			{
				PlayerController->RobotHUDUserWidget->SetHealth(RobotPawn->HealthComponent->Health, RobotPawn->HealthComponent->DefaultHealth);
			}
		}
	}
}
