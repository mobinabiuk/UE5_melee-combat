// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"
#include"DrawDebugHelpers.h"
#include "UE5_MeleeCombat/DebugMacros.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"

//#define Draw_Debug_Sphere(World,Location) DrawDebugSphere(World,Location,70.f,24,FColor::Red,false,3.f);

AItem::AItem()
{
	PrimaryActorTick.bCanEverTick = true;
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	RootComponent = MeshComponent;

	SphereMesh = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	SphereMesh->SetupAttachment(GetRootComponent());
}


void AItem::BeginPlay()
{
	Super::BeginPlay();

	SphereMesh->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnSphereBeginOverlap);
	SphereMesh->OnComponentEndOverlap.AddDynamic(this, &AItem::OnSphereEndOverlap);
	
}



void AItem::OnSphereBeginOverlap( UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	const FString OtherActorName = OtherActor->GetName();
	GEngine->AddOnScreenDebugMessage(1, 3.f, FColor::Green, OtherActorName);
}

void AItem::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	const FString OtherActerName = OtherActor->GetName();
	GEngine->AddOnScreenDebugMessage(1, 3.f, FColor::Red, OtherActerName);

}


void AItem::Tick(float DeltaTime)
{

	Super::Tick(DeltaTime);
	
}

