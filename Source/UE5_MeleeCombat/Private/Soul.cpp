// Fill out your copyright notice in the Description page of Project Settings.


#include "Soul.h"

void ASoul::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASoul::BeginPlay()
{
	Super::BeginPlay();
}

void ASoul::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnSphereBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	
}
