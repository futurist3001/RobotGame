#include "RobotGame/public/Other/RushItem.h"

#include "RobotGame/public/Gameplay/Pawn/RobotPawn.h"

ARushItem::ARushItem(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;

	BonusSpeed = 10.f;
	TimeResetSpeed = 10.f;

	bIsUnderRush = false;
}

void ARushItem::SpecialBonus(AActor* Actor)
{
	if (Actor->IsValidLowLevel(); ARobotPawn* RobotPawn = Cast<ARobotPawn>(Actor))
	{
		if (!bIsUnderRush)
		{
			bIsUnderRush = true;

			float DefaultSpeed = RobotPawn->Speed;
			RobotPawn->Speed += BonusSpeed;

			FTimerHandle ResetSpeedTimer;
			GetWorld()->GetTimerManager().SetTimer(
				ResetSpeedTimer, [this, RobotPawn, DefaultSpeed]()
				{
					RobotPawn->Speed = DefaultSpeed;
					bIsUnderRush = false;
				}, TimeResetSpeed, false);
		}
	}
}
