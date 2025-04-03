// Fill out your copyright notice in the Description page of Project Settings.


#include "BreakableActor.h"
#include "Treasure.h"
#include "Components/CapsuleComponent.h"
#include "GeometryCollection/GeometryCollectionComponent.h"


ABreakableActor::ABreakableActor()
{
	PrimaryActorTick.bCanEverTick = false;
	GeometryCollection = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("GeometryCollection"));
	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleCollision"));
	Capsule->SetupAttachment(GetRootComponent());
	SetRootComponent(GeometryCollection);
	GeometryCollection->SetGenerateOverlapEvents(true);
	Capsule->SetCollisionResponseToAllChannels(ECR_Ignore);
	Capsule->SetCollisionResponseToChannel(ECC_Pawn,ECR_Block);
}

void ABreakableActor::BeginPlay()
{
	Super::BeginPlay();
}

void ABreakableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

	void ABreakableActor::GetHit_Implementation(const FVector& ImpactPoint)
{
	UWorld* World = GetWorld();
	if (IsValid(World) && TreasureClasses.Num() > 0)
	{
		FVector Location = GetActorLocation();
		Location.Z += 75.f;
		const int32 Selection = FMath::RandRange(0,TreasureClasses.Num() - 1);
		World->SpawnActor<ATreasure>(TreasureClasses[Selection],Location,GetActorRotation());
	}
}
