// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Weapon.generated.h"

class UBoxComponent;

/**
 * 
 */
UCLASS()
class UE5_MELEECOMBAT_API AWeapon : public AItem
{
	GENERATED_BODY()
	
public:
	AWeapon();
	// Called every frame
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	void Equip(USceneComponent* InParent,FName InSocketName);

	void AttachMeshToSocket(USceneComponent* InParent, const FName& InSocketName);

protected: 

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sine Parameters", meta = (AllowPrivateAccess = "true"))
	//float Amplitude = 0.5f;  // Adjust the amplitude as needed

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sine Parameters", meta = (AllowPrivateAccess = "true"))
	//float TimeConstant = 5.0f;  // Adjust the time constant as needed

	float RunningTime = 0.0f;

	/*float CalculateSinusoidalOffset();*/


	virtual	void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)override;

	virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)override;

private:
	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	UBoxComponent* WeaponBox;
};
