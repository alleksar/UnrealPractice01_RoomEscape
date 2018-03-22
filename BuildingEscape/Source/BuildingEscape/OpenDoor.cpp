// Copyright Aleksa Raicevic 2018

#include "OpenDoor.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"


// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UOpenDoor::OpenDoor()
{
	FRotator NewRotator = FRotator(0.0f, -70.0f, 0.0f);
	OwnerActor->SetActorRotation(NewRotator);
}

void UOpenDoor::CloseDoor()
{

	FRotator NewRotator = FRotator(0.0f, 0.0f, 0.0f);
	OwnerActor->SetActorRotation(NewRotator);
}

// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
	OwnerActor = GetOwner();
	ActorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();

	
	// ...
	
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (TriggerPlate->IsOverlappingActor(ActorThatOpens))
	{
		OpenDoor();
		DoorLastOpenTime = GetWorld()->GetTimeSeconds();
	}

	if (GetWorld()->GetTimeSeconds() > DoorLastOpenTime + DoorCloseDelay)
	{
		CloseDoor();
	}

	

	
	// ...
}

