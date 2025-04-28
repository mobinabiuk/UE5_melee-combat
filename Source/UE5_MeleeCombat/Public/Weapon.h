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
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	void DeactivateEmbers();
	void Equip(USceneComponent* InParent,FName InSocketName,AActor* NewOwner,APawn* NewInstigator);
	
	void AttachMeshToSocket(USceneComponent* InParent, const FName& InSocketName);
	void ExecuteGetHit(FHitResult BoxHit);
	bool ActorIsSameType(AActor* OtherActor);
	TArray<AActor*> IgnoreActors;

protected:
	
	float RunningTime = 0.0f;

	UFUNCTION()
	void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintImplementableEvent)
	void CreateFields(const FVector& FieldLocation);
	
private:
	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	UBoxComponent* WeaponBox;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* BoxTraceStart;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* BoxTraceEnd;

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	float Damage = 20.f;

	void BoxTrace(FHitResult& BoxHit);

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	FVector BoxTraceExtent=FVector(5.f);

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	bool bShowBoxDebug=false;
	
public:
	FORCEINLINE UBoxComponent* GetWeaponBox() const { return WeaponBox; }
};


