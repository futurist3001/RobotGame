#include "Other/HealthComponent.h"

UHealthComponent::UHealthComponent(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = false;

	DefaultHealth = 100.f;
}

void UHealthComponent::TakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (Damage <= 0)
	{
		return;
	}

	Health = FMath::Clamp(Health - Damage, 0.0f, DefaultHealth);

	HealthChanged.Broadcast(GetOwner(), this);
}

void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	
	Health = DefaultHealth;
	AActor* Owner = GetOwner();

	if (Owner->IsValidLowLevel())
	{
		Owner->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::TakeDamage);
	}
}

void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

