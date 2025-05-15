// Fill out your copyright notice in the Description page of Project Settings.


#include "Treasure.h"
#include "SlashCharacter.h"
#include "Kismet/GameplayStatics.h"

void ATreasure::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                     UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ASlashCharacter* SlashCharcter = Cast<ASlashCharacter>(OtherActor);
	if (IsValid(SlashCharcter))
	{
		//playing pickup sound
		SpawnPickupSound();
		Destroy();
	}
}
