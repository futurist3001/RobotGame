#include "RobotGame/public/Other/OverchargeItem.h"

#include "RobotGame/public/Gameplay/Pawn/RobotPawn.h"
#include "RobotGame/public/Gameplay/Core/RobotPlayerController.h"

AOverchargeItem::AOverchargeItem(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;

	BonusEnergy = 10.f;
	//TimeOverChargeEnd = 10.f; // old logic
}

void AOverchargeItem::SpecialBonus(AActor* Actor)
{
	if (Actor->IsValidLowLevel(); ARobotPawn * RobotPawn = Cast<ARobotPawn>(Actor))
	{
		float StartEnergyCount = RobotPawn->GetCurrentEnergy();

		if (StartEnergyCount < RobotPawn->DefaultEnergy)
		{
			RobotPawn->SetCurrentEnergy(RobotPawn->GetCurrentEnergy() + BonusEnergy);

			// old logic
			/*FTimerHandle EndOverchargeTimer;
			GetWorld()->GetTimerManager().SetTimer(
				EndOverchargeTimer, [RobotPawn, StartEnergyCount]()
				{
					RobotPawn->SetCurrentEnergy(StartEnergyCount);
					if (ARobotPlayerController* PlayerController = Cast<ARobotPlayerController>(RobotPawn))
					{
						if (PlayerController->RobotHUDUserWidget)
						{
							PlayerController->UpdateHUDWidget();
						}
					}
				}, TimeOverChargeEnd, false);*/
		}
	}
}
