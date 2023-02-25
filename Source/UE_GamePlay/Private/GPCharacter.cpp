// Fill out your copyright notice in the Description page of Project Settings.


#include "GPCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Particles/ParticleEventManager.h"
#include "Kismet/KismetMathLibrary.h"

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

const FVector AGPCharacter::OrientDirection()
{
	// Orient the attack direction towards to CrossHair by Line trace
	FRotator CameraRotator = CameraComp->GetComponentRotation();
	FVector CameraLocation = CameraComp->GetComponentLocation();
	FVector CameraEnd = CameraLocation + CameraRotator.Vector() * 10000;

	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);
	ObjectQueryParams.AddObjectTypesToQuery(ECC_PhysicsBody);

	FHitResult Hit;
	bool Hit_result = GetWorld()->LineTraceSingleByObjectType(Hit, CameraLocation, CameraEnd, ObjectQueryParams);
	FVector end = Hit_result ? Hit.ImpactPoint : Hit.TraceEnd;

	//FColor DebugColor = Hit_result ? FColor::Green : FColor::Orange;
	//DrawDebugLine(GetWorld(), HandLocation, Hit.ImpactPoint, DebugColor, false, 2.0f, 0, 1.5f);

	return end;
}

// Called to bind functionality to input
void AGPCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AGPCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AGPCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAction("MagicBullet", IE_Pressed, this, &AGPCharacter::Attack_MagicBullet);
	PlayerInputComponent->BindAction("BlackHole", IE_Pressed, this, &AGPCharacter::Attack_BlackHole);
	PlayerInputComponent->BindAction("Teleport", IE_Pressed, this, &AGPCharacter::Attack_Teleport);
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

// Attack: Magic Bullet
void AGPCharacter::Attack_MagicBullet()
{
	PlayAnimMontage(AnimMontage);
	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &AGPCharacter::CallingAttack_MB, 0.2f);
}

void AGPCharacter::CallingAttack_MB()
{
	Attack(MagicBullet);
}

// Attack: Black Hole
void AGPCharacter::Attack_BlackHole()
{
	PlayAnimMontage(AnimMontage);
	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &AGPCharacter::CallingAttack_BH, 0.2f);
}

void AGPCharacter::CallingAttack_BH()
{
	Attack(BlackHole);
}

// Attack: Teleport
void AGPCharacter::Attack_Teleport()
{
	PlayAnimMontage(AnimMontage);
	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &AGPCharacter::CallingAttack_TP, 0.2f);
}

void AGPCharacter::CallingAttack_TP()
{
	Attack(Teleport);
}

// Basic attack function
void AGPCharacter::Attack(TSubclassOf<AActor> AttackName)
{
	FVector HandLocation = GetMesh()->GetSocketLocation("Middle_Attack");
	FVector end = AGPCharacter::OrientDirection();
	
	FRotator OrientRotation = UKismetMathLibrary::FindLookAtRotation(HandLocation, end);

	FTransform SpawnTransform = FTransform(OrientRotation, HandLocation);

	FActorSpawnParameters SpawnParas;
	SpawnParas.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParas.Instigator = this;

	GetWorld()->SpawnActor<AActor>(AttackName, SpawnTransform, SpawnParas);
}

