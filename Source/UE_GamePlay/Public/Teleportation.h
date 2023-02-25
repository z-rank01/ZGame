// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseProjectile.h"
#include "Teleportation.generated.h"

/**
 * 
 */
UCLASS()
class UE_GAMEPLAY_API ATeleportation : public ABaseProjectile
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UParticleSystem* EmitterTemplate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FTimerHandle TimerHandle_Teleport;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FTimerHandle TimerHandle_Explode;

	UPROPERTY(EditAnywhere)
	float DelayTime = 2.0f;

	// Must use UFUNCTION to bind the event
	UFUNCTION()
	void OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	// Override base projectile BeginPlay function
	virtual void BeginPlay() override;

	void Explode();
	void Teleport();
};
