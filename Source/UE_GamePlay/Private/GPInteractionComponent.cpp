// Fill out your copyright notice in the Description page of Project Settings.


#include "GPInteractionComponent.h"
#include "GPInterface.h"

// Sets default values for this component's properties
UGPInteractionComponent::UGPInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGPInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UGPInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UGPInteractionComponent::PrimaryInteraction()
{
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);

	AActor* MyOwner = GetOwner();

	FVector Eye_Location;
	FRotator Eye_Rotation;
	MyOwner->GetActorEyesViewPoint(Eye_Location, Eye_Rotation);
	FVector start = Eye_Location;
	FVector end = Eye_Location + (Eye_Rotation.Vector() * 1000);

	// Interaction Condition: Line Trace (same as ray casting)
	// if character is not looking at the object, it wont trigger.
	// Return bool indicates whether it hit, and 'Hit' is changed as the result of the function as well.
	//FHitResult Hit;
	//bool Hit_result = GetWorld()->LineTraceSingleByObjectType(Hit, start, end, ObjectQueryParams);

	float Radius = 30.0f;
	TArray<FHitResult> Hits;
	FCollisionShape Shape;
	Shape.SetSphere(Radius);
	bool Hits_result = GetWorld()->SweepMultiByObjectType(Hits, start, end, FQuat::Identity, ObjectQueryParams, Shape);

	FColor DebugColor = Hits_result ? FColor::Green : FColor::Orange;

	// Interaction:
	// Hit_Actor: Detect whether character or player is looking at the object
	for (FHitResult Hit : Hits)
	{
		AActor* Hit_Actor = Hit.GetActor();
		if (Hit_Actor)
		{
			if (Hit_Actor->Implements<UGPInterface>())
			{
				APawn* MyPawn = Cast<APawn>(MyOwner);
				IGPInterface::Execute_Interact(Hit_Actor, MyPawn);
			}
		}
		DrawDebugSphere(GetWorld(), Hit.ImpactPoint, Radius, 32, DebugColor, false, 2.0f);
	}
	DrawDebugLine(GetWorld(), start, end, DebugColor, false, 2.0f, 0, 1.5f);
}
