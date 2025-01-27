#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"

#include "RobotPlayerController.generated.h"

class URobotHUDUserWidget;

UCLASS()
class ROBOTGAME_API ARobotPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<URobotHUDUserWidget> RobotHUDUserWidgetClass;

	UPROPERTY(Transient, BlueprintReadOnly)
	TObjectPtr<URobotHUDUserWidget> RobotHUDUserWidget;

public:

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	void CreateRobotHUDUserWidget();
	void UpdateHUDWidget();
};
