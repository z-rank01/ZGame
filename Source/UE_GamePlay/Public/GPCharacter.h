// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GPInteractionComponent.h"
#include "GPCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UGPInteractionComponent;

UCLASS()
class UE_GAMEPLAY_API AGPCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AGPCharacter();

protected:

	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> MagicBullet;

	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> BlackHole;

	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> Teleport;

	UPROPERTY(EditAnywhere, Category = "Attack")
	UAnimMontage* AnimMontage;

	FTimerHandle TimerHandle_PrimaryAttack;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArmComp;
	
	UPROPERTY(VisibleAnywhere)
	UGPInteractionComponent* InteractionComp;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward(float value);
	void MoveRight(float value);
	void Attack(TSubclassOf<AActor> AttackName);
	void PrimaryInteract();
	const FVector OrientDirection();

	void Attack_MagicBullet();
	void CallingAttack_MB();	

	void Attack_BlackHole();
	void CallingAttack_BH();

	void Attack_Teleport();
	void CallingAttack_TP();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
