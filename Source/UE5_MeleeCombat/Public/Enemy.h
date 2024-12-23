// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "HitInterface.h"
#include "Enemy.generated.h"

class UAnimMontage;
class USoundBase;


UCLASS()
class UE5_MELEECOMBAT_API AEnemy : public ACharacter,public IHitInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void GetHit(const FVector& ImpactPoint)override;
	void DirectionalHitReact(const FVector& ImpactPoint);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/*
	* play montage functions
	*/
	void PlayHitReactMontage(const FName& SectionName);

private:
	/*
	* Anim Montages
	*/
	UPROPERTY(EditDefaultsOnly, Category = "Montages")
	UAnimMontage* HitReactMontage;

	UPROPERTY(EditAnywhere,Category=Sounds)
	USoundBase* HitSound;

	UPROPERTY(EditAnywhere, Category = VisualEffects)
	UParticleSystem* HitParticles;

};
