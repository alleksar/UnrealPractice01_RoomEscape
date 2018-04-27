// Copyright Aleksa Raicevic 2018

#include "WallOpen.h"


// Sets default values for this component's properties
UWallOpen::UWallOpen()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UWallOpen::BeginPlay()
{
	Super::BeginPlay();
	OwnerActor = GetOwner();
	if (!TriggerPlate)
	{
		UE_LOG(LogTemp, Error, TEXT("%s is missing pressure plate"), *GetOwner()->GetName());
	}
	// ...
	
}


// Called every frame
void UWallOpen::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (GetTotalMassOfActorsOnPlate()>TriggerMass)
	{
		OnWallOpen.Broadcast();
	}
	else
	{
		OnWallClose.Broadcast();
	}
	// ...
}

float UWallOpen::GetTotalMassOfActorsOnPlate()
{
	float TotalMass = 0.0f;
	TArray<AActor*> OverlappingActors;
	if (!TriggerPlate) { return TotalMass; }
	TriggerPlate->GetOverlappingActors(OUT OverlappingActors);

	for (const auto *Actor : OverlappingActors)
	{
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}

	return TotalMass;
}

