// Fill out your copyright notice in the Description page of Project Settings.


#include "GPCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Particles/ParticleEventManager.h"

// Sets default values
AGPCharacter::AGPCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->bUsePawnControlRotation = true;

	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	CameraComp->SetupAttachment(SpringArmComp);

	InteractionComp = CreateDefaultSubobject<UGPInteractionComponent>("InteractionComp");

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
}

// Called when the game starts or when spawned
void AGPCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGPCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AGPCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AGPCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AGPCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &AGPCharacter::Attack);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AGPCharacter::Jump);
	PlayerInputComponent->BindAction("PrimaryInteract", IE_Pressed, this, &AGPCharacter::PrimaryInteract);
}

// Movement Function
void AGPCharacter::MoveForward(float value)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;

	AddMovementInput(ControlRot.Vector(), value);
}

void AGPCharacter::MoveRight(float value)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;

	FVector RightVector = FRotationMatrix(ControlRot).GetScaledAxis(EAxis::Y);

	AddMovementInput(RightVector, value);
}

void AGPCharacter::PrimaryInteract()
{
	InteractionComp->PrimaryInteraction();
}

void AGPCharacter::Attack()
{
	PlayAnimMontage(AnimMontage);
	
	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &AGPCharacter::Attack_TimeDelay, 0.2f);
}

void AGPCharacter::Attack_TimeDelay()
{
	FVector HandLocation = GetMesh()->GetSocketLocation("Middle_Attack");
	FTransform SpawnTransform = FTransform(GetControlRotation(), HandLocation);

	FActorSpawnParameters SpawnParas;
	SpawnParas.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParas.Instigator = this;

	GetWorld()->SpawnActor<AActor>(MagicClass, SpawnTransform, SpawnParas);
}
