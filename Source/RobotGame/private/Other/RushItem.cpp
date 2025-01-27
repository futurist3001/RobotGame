#include "RobotGame/public/Other/RushItem.h"

#include "RobotGame/public/Gameplay/Pawn/RobotPawn.h"

ARushItem::ARushItem(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;

	BonusSpeed = 30.f;
	TimeResetSpeed = 15.f;
}

void ARushItem::SpecialBonus(AActor* Actor)
{
	if (Actor->IsValidLowLevel(); ARobotPawn* RobotPawn = Cast<ARobotPawn>(Actor))
	{
		if (RobotPawn->Speed != BonusSpeed)
		{
			float DefaultSpeed = RobotPawn->Speed;
			RobotPawn->Speed = BonusSpeed;

			FTimerHandle ResetSpeedTimer;
			GetWorld()->GetTimerManager().SetTimer(
				ResetSpeedTimer, [this, RobotPawn, DefaultSpeed]()
				{
					RobotPawn->Speed = DefaultSpeed;
				}, TimeResetSpeed, false);
		}
	}
}
