// Fill out your copyright notice in the Description page of Project Settings.


#include "ExplosiveBarrel.h"
#include "DrawDebugHelpers.h"

// Sets default values
AExplosiveBarrel::AExplosiveBarrel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComp");
	StaticMeshComp->SetSimulatePhysics(true);
	RootComponent = StaticMeshComp;

	RadialForceComp = CreateDefaultSubobject<URadialForceComponent>("RadialForceComp");
	RadialForceComp->SetupAttachment(StaticMeshComp);
	RadialForceComp->SetAutoActivate(false);
	RadialForceComp->Radius = 500.0f;
	RadialForceComp->ImpulseStrength = 200.0f;
	RadialForceComp->bImpulseVelChange = true;

	// Add additional type of object to collide with
	RadialForceComp->AddCollisionChannelToAffect(ECC_WorldDynamic);
}

void AExplosiveBarrel::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	StaticMeshComp->OnComponentHit.AddDynamic(this, &AExplosiveBarrel::OnActorHit);
}

// Called when the game starts or when spawned
void AExplosiveBarrel::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AExplosiveBarrel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AExplosiveBarrel::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalInpulse, const FHitResult& Hit)
{
	RadialForceComp->FireImpulse();

	// Use 'UE_LOG' for displaying debug log
	UE_LOG(LogTemp, Log, TEXT("Func.OnActionHit: Trigger"));
	UE_LOG(LogTemp, Warning, TEXT("Func.OnActionHit: Trigger data for %s, at %f"), *GetNameSafe(OtherActor), GetWorld()->TimeSeconds);

	// Use 'DrawDebug_' function to display debug log in 3D viewer
	DrawDebugString(GetWorld(), Hit.ImpactPoint, TEXT("Triggering correctly"), nullptr, FColor::Green, 2.0f, true, 1.5f);
}