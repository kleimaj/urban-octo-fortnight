// Copyright Dirty Minds Combined 2021


#include "Grabber.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	FindPhysicsHandle();
	SetupInputComponent();
}

void UGrabber::SetupInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent) {
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
}

void UGrabber::FindPhysicsHandle()
{
	// Check for PhysicsHandleComponent
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (!PhysicsHandle) {
		UE_LOG(LogTemp, Error, TEXT("Physics Handle not attached to %s"), *GetOwner()->GetName());
	}
}

void UGrabber::Grab()
{
	UE_LOG(LogTemp, Warning, TEXT("Grabber pressed"));
	
		// Initialize FVector and FRotation
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	// Populate player's vierpoint
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint( 
		OUT PlayerViewPointLocation, 
		OUT PlayerViewPointRotation
		);
	
	FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;

	FHitResult HitResult = GetFirstPhysicsBodyInReach();
	UPrimitiveComponent* ComponentToGrab = HitResult.GetComponent();

	// If we hit something then attach the physics handle
	if (HitResult.GetActor()) {
		PhysicsHandle->GrabComponentAtLocation(
			ComponentToGrab,
			NAME_None,
			LineTraceEnd
		);
	}

	// TODO attach physics handle
}

void UGrabber::Release()
{
	UE_LOG(LogTemp, Warning, TEXT("Grabber released"));
	
	// TODO release/remove physics handle
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Initialize FVector and FRotation
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	// Populate player's vierpoint
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint( 
		OUT PlayerViewPointLocation, 
		OUT PlayerViewPointRotation
		);
	
	FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;

	// If physics handle is attached
	if (PhysicsHandle->GrabbedComponent) {
		PhysicsHandle->SetTargetLocation(LineTraceEnd);
	}
		// Move the object we are holding

}

FHitResult UGrabber::GetFirstPhysicsBodyInReach() const
{
	// Initialize FVector and FRotation
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	// Populate player's vierpoint
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint( 
		OUT PlayerViewPointLocation, 
		OUT PlayerViewPointRotation
		);
	
	FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;
	// FVector LineTraceDirection = PlayerViewPointRotation.Vector();
	
	// DrawDebugLine(
	// 	GetWorld(),
	// 	PlayerViewPointLocation,
	// 	LineTraceEnd,
	// 	FColor(0,255,0),
	// 	false,
	// 	0.f,
	// 	0,
	// 	5
	// );

	// Ray-cast out at to a certain distance (Reach)
	FHitResult Hit;
	FCollisionQueryParams TraceParams(
		FName(TEXT("")),
		false,
		GetOwner()
	);
	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		PlayerViewPointLocation,
		LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParams
	);
	// See what the ray-cast collides with
	if (Hit.GetActor()){
		UE_LOG(LogTemp, Warning, TEXT("%s"), *Hit.GetActor()->GetName());
	}

	return Hit;
}