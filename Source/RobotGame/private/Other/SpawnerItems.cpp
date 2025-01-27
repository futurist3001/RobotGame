#include "RobotGame/public/Other/SpawnerItems.h"

#include "Components/SphereComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

#include "RobotGame/public/Gameplay/Pawn/RobotPawn.h"

ASpawnerItems::ASpawnerItems(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Collision Component"));
	ItemVFX = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Niagara Item"));

	SphereComponent->InitSphereRadius(70.f);

	SetRootComponent(SphereComponent);
	ItemVFX->SetupAttachment(RootComponent);

	ItemVFX->SetAutoActivate(true);

	RespawnTime = 10.0f;
}

void ASpawnerItems::Respawn()
{
	ItemVFX->SetVisibility(true);
	//ItemVFX->Activate();

	SetActorEnableCollision(true);
}

void ASpawnerItems::SpecialBonus(AActor* Actor)
{
}

void ASpawnerItems::BeginPlay()
{
	Super::BeginPlay();

	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ASpawnerItems::OnBeginOverlap);
}

void ASpawnerItems::OnBeginOverlap(
	UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsValidLowLevel() && OtherActor != this && OtherActor->IsA<ARobotPawn>())
	{
		SpecialBonus(OtherActor);

		ItemVFX->SetVisibility(false);
		//ItemVFX->Deactivate();

		SetActorEnableCollision(false);

		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(), DeathVFX, GetActorLocation());

		GetWorld()->GetTimerManager().SetTimer(
			RespawnTimerHandle, this, &ASpawnerItems::Respawn, RespawnTime, false);
	}
}

