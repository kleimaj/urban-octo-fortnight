// Copyright Dirty Minds Combined 2021


#include "OpenDoor.h"
#include "Components/PrimitiveComponent.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Actor.h"

#define OUT

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	InitialYaw = GetOwner()->GetActorRotation().Yaw;
	CurrentYaw = InitialYaw;
	OpenAngle += InitialYaw;

	if (!PressurePlate) {
		UE_LOG(LogTemp, Error, TEXT("%s has OpenDoor component but no PressurePlate set"), *GetOwner()->GetName());
	}
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (TotalMassOfActors() > MassToOpenDoors) {
		OpenDoor(DeltaTime);
		DoorLastOpened = GetWorld()->GetTimeSeconds();
	}
	else {
		if (GetWorld()->GetTimeSeconds() - DoorLastOpened > DoorCloseDelay) {
			CloseDoor(DeltaTime);
		}
	}

	
}

void UOpenDoor::OpenDoor(float DeltaTime)
{
	FRotator OpenDoor = GetOwner()->GetActorRotation();
	CurrentYaw = FMath::FInterpTo(CurrentYaw, OpenAngle, DeltaTime, 2);
	OpenDoor.Yaw = CurrentYaw;

	GetOwner()->SetActorRotation(OpenDoor);
}


void UOpenDoor::CloseDoor(float DeltaTime)
{
	FRotator OpenDoor = GetOwner()->GetActorRotation();
	CurrentYaw = FMath::FInterpTo(CurrentYaw, InitialYaw, DeltaTime, 2);
	OpenDoor.Yaw = CurrentYaw;

	GetOwner()->SetActorRotation(OpenDoor);
}

float UOpenDoor::TotalMassOfActors() const
{
	float TotalMass = 0.f;

	// Find all overlapping actors
	TArray<AActor*> OverlappingActors;
	if (!PressurePlate) return TotalMass;
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);

	// sum up their masses
	for (AActor* Actor : OverlappingActors) {
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}
	return TotalMass;
}