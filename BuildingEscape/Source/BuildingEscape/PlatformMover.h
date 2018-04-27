// Copyright Aleksa Raicevic 2018

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"
#include "PlatformMover.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPlatformMoveRequest);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UPlatformMover : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlatformMover();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	UPROPERTY(BlueprintAssignable)
	FPlatformMoveRequest OnPlatformMove;
	UPROPERTY(BlueprintAssignable)
	FPlatformMoveRequest OnPlatformStay;

private:
	UPROPERTY(EditAnywhere)
	ATriggerVolume *TriggerPlate = nullptr;
	AActor *OwnerActor = nullptr;
	UPROPERTY(EditAnywhere)
	float TriggerMass = 50.f;

	float GetTotalMassOfActorsOnPlate();
		
	
};
