// Fill out your copyright notice in the Description page of Project Settings.


#include "Teleportation.h"
#include "Kismet/GameplayStatics.h"

void ATeleportation::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor != GetInstigator())
	{
		Explode();
		GetWorldTimerManager().SetTimer(TimerHandle_Teleport, this, &ATeleportation::Teleport, .2f);
	}
}

void ATeleportation::BeginPlay()
{
	Super::BeginPlay();

	// Prevent colliding Player
	SphereComp->IgnoreActorWhenMoving(GetInstigator(), true);
	// Bind a UObject instance and a UFUNCTION to a dynamic multi-cast delegate
	SphereComp->OnComponentHit.AddDynamic(this, &ATeleportation::OnActorHit);

	GetWorldTimerManager().SetTimer(TimerHandle_Explode, this, &ATeleportation::Explode, DelayTime);
	//GetWorldTimerManager().ClearTimer(TimerHandle_Teleport);
	GetWorldTimerManager().SetTimer(TimerHandle_Teleport, this, &ATeleportation::Teleport, DelayTime + .2f);
}

void ATeleportation::Explode()
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), EmitterTemplate, GetActorLocation(), GetActorRotation());

	MovementComp->InitialSpeed = 0.0f;
}

void ATeleportation::Teleport()
{
	FVector ProjectileLocation = GetActorLocation();
	FRotator ProjectileRotation = GetInstigator()->GetActorRotation();
	ProjectileRotation.Yaw = GetActorRotation().Yaw;

	GetInstigator()->SetActorLocationAndRotation(ProjectileLocation, ProjectileRotation);

	Destroy(true);
}
