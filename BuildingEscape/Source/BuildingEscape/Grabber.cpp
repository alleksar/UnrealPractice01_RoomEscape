// Copyright Aleksa Raicevic 2018

#include "Grabber.h"
#include "Engine/World.h"
#include "GameFramework/Controller.h"
#include "GameFramework/Actor.h"
#include "DrawDebugHelpers.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	FindPhysicsHandleComponent();
	SetupInputComponent();
	
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//UE_LOG(LogTemp, Warning, TEXT("Location: %s Rocation: %s"), *PlayerViewpointLocation.ToString(), *PlayerViewpointRotation.ToString())
	//DrawDebugLine(GetWorld(), PlayerViewpointLocation, LineTracer, FColor(255, 0, 0), false, 0.f, 0.f,10.f);
	
	if (!PhysicHandle) { return; }
	if (PhysicHandle->GrabbedComponent)
	{
		PhysicHandle->SetTargetLocation(GetReachLineEnd());
	}
	

}

void UGrabber::Grab()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab pick up"))
		
	auto HitResult = GetFirstPhysicsBodyInReach();
	auto ComponentToGrab = HitResult.GetComponent();
	auto ActorHit = HitResult.GetActor();

	if (ActorHit)
	{
		if (!PhysicHandle) { return; }
		PhysicHandle->GrabComponent(
			ComponentToGrab,
			NAME_None,
			ComponentToGrab->GetOwner()->GetActorLocation(),
			true
			);
	}
	
}

void UGrabber::Release()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab release"))

	GetFirstPhysicsBodyInReach();
	if (!PhysicHandle) { return; }
	PhysicHandle->ReleaseComponent();

}

void UGrabber::FindPhysicsHandleComponent()
{
	PhysicHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	

	if (PhysicHandle == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("%s missing physic handle component"), *GetOwner()->GetName());
	}
}

void UGrabber::SetupInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent)
	{
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("%s is missing input component"), *GetOwner()->GetName());
	}

}

FHitResult UGrabber::GetFirstPhysicsBodyInReach() const
{
	FCollisionQueryParams TraceParamaters(FName(TEXT("")), false, GetOwner());
	FHitResult Hit;

	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		GetReachLineStart(),
		GetReachLineEnd(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParamaters
	);

	AActor *ActorHit = Hit.GetActor();

	if (ActorHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("Line trace hit: %s"), *(ActorHit->GetName()))
	}
	return Hit;

}


FVector UGrabber::GetReachLineEnd() const
{
	FVector PlayerViewpointLocation;
	FRotator PlayerViewpointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewpointLocation, OUT PlayerViewpointRotation);

	return PlayerViewpointLocation + PlayerViewpointRotation.Vector()*Reach;
}

FVector UGrabber::GetReachLineStart() const
{
	FVector PlayerViewpointLocation;
	FRotator PlayerViewpointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewpointLocation, OUT PlayerViewpointRotation);

	return PlayerViewpointLocation;
}
