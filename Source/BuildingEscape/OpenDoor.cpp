// Copyright Dirty Minds Combined 2021


#include "OpenDoor.h"
#include "GameFramework/Actor.h"

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

	UE_LOG(LogTemp, Warning, TEXT("%s"), *GetOwner()->GetActorRotation().ToString());
	UE_LOG(LogTemp, Warning, TEXT("%f"), TargetYaw);


	// float MyFloat = 90.f;

	// FRotator CurrentRotation = GetOwner()->GetActorRotation();

	// CurrentRotation.Yaw = MyFloat;
	// GetOwner()->SetActorRotation(CurrentRotation);
	
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UE_LOG(LogTemp, Warning, TEXT("%s"), *GetOwner()->GetActorRotation().ToString());
	UE_LOG(LogTemp, Warning, TEXT("%f"), TargetYaw);

	float CurrentYaw = GetOwner()->GetActorRotation().Yaw;
	
	FRotator OpenDoor { 0.f, 0.f, 0.f };

	OpenDoor.Yaw = FMath::Lerp(CurrentYaw, TargetYaw, 0.02f);

	GetOwner()->SetActorRotation(OpenDoor);
}

