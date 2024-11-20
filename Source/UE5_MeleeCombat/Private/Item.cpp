// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"
#include"DrawDebugHelpers.h"
#include "UE5_MeleeCombat/DebugMacros.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "SlashCharacter.h"

//#define Draw_Debug_Sphere(World,Location) DrawDebugSphere(World,Location,70.f,24,FColor::Red,false,3.f);

AItem::AItem()
{
	PrimaryActorTick.bCanEverTick = true;
	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	RootComponent = ItemMesh;

	SphereMesh = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	SphereMesh->SetupAttachment(GetRootComponent());
}


void AItem::BeginPlay()
{
	Super::BeginPlay();

	SphereMesh->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnSphereBeginOverlap);
	SphereMesh->OnComponentEndOverlap.AddDynamic(this, &AItem::OnSphereEndOverlap);
	
}

//float AIItem::TransformedSin()
//{
//	return Amplitude * FMath::Sin(RunningTime * TimeConstant);
//}
//
//float AIItem::TransformedCos()
//{
//	return Amplitude * FMath::Cos(RunningTime * TimeConstant);
//}



void AItem::OnSphereBeginOverlap( UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ASlashCharacter* SlashCharacter = Cast<ASlashCharacter>(OtherActor);
	if (SlashCharacter)
	{
		SlashCharacter->SetOverlappingItem(this);
	}

}

void AItem::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ASlashCharacter* SlashCharacter = Cast<ASlashCharacter>(OtherActor);
	if (SlashCharacter)
	{
		SlashCharacter->SetOverlappingItem(nullptr);
	}

}


void AItem::Tick(float DeltaTime)
{

	Super::Tick(DeltaTime);
	/*RunningTime += DeltaTime;*/
}

