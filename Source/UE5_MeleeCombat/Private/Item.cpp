// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"
#include"DrawDebugHelpers.h"
#include "UE5_MeleeCombat/DebugMacros.h"
//#define Draw_Debug_Sphere(World,Location) DrawDebugSphere(World,Location,70.f,24,FColor::Red,false,3.f);

AItem::AItem()
{
	PrimaryActorTick.bCanEverTick = true;

}


void AItem::BeginPlay()
{
	Super::BeginPlay();
	int16 MinusFunctionRes = Minus<int16>(20, 10);
	GEngine->AddOnScreenDebugMessage(0, 5.f, FColor::Green, FString::Printf(TEXT("Minus Function: %d"), MinusFunctionRes));
}


void AItem::Tick(float DeltaTime)
{

	Super::Tick(DeltaTime);
	
	//AddActorWorldOffset());
	Draw_Debug_Sphere(GetActorLocation());
}

