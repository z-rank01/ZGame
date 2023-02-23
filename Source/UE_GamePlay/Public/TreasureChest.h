// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "GPInterface.h"
#include "TreasureChest.generated.h"

UCLASS()
class UE_GAMEPLAY_API ATreasureChest : public AActor, public IGPInterface
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	float RoateValue;

	void Interact_Implementation(APawn* TriggerPawn);
	
public:	
	// Sets default values for this actor's properties
	ATreasureChest();

protected:
	
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* BaseMesh;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* LidMesh;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
