// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HitInterface.h"
#include "BreakableActor.generated.h"

class UGeometryCollectionComponent;

UCLASS()
class UE5_MELEECOMBAT_API ABreakableActor : public AActor,public IHitInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABreakableActor();
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter)override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/* capsul collision*/
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	class UCapsuleComponent* Capsule;

private:
	UPROPERTY(VisibleAnywhere);
	UGeometryCollectionComponent* GeometryCollection;

	UPROPERTY(EditAnywhere,Category = "Treasure Properties")
	TArray<TSubclassOf<class ATreasure>> TreasureClasses;
};
