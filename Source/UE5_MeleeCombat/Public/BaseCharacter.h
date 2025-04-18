// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "HitInterface.h"
#include "BaseCharacter.generated.h"

class AWeapon;
class UAttributeComponent;
class UAnimMontage;

UCLASS()
class UE5_MELEECOMBAT_API ABaseCharacter : public ACharacter,public IHitInterface
{
	GENERATED_BODY()

public:
	ABaseCharacter();
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void SetWeaponCollisionEnabled(ECollisionEnabled::Type CollisionEnabled);

protected:
	virtual void BeginPlay() override;

	virtual void Attack();
	/*play montage functions*/
	virtual void PlayAttackMontage();
	void PlayHitReactMontage(const FName& SectionName);
	void DirectionalHitReact(const FVector& ImpactPoint);
	
	virtual bool CanAttack();
	
	UFUNCTION(BlueprintCallable)
	virtual void AttackEnd();

	virtual void Die();
	
	UPROPERTY(VisibleAnyWhere,Category= Weapon)
	AWeapon* EquippedWeapon;
	/*
	* Anim Montages
	*/
	UPROPERTY(EditDefaultsOnly,Category ="Montages")
	UAnimMontage* AttackMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Montages")
	UAnimMontage* HitReactMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Montages")
	UAnimMontage* DeathMontage;

	
	/*Components*/
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,meta=(AllowPrivateAccess=true))
	UAttributeComponent* Attributes;

	
	UPROPERTY(EditAnywhere,Category=Sounds)
	USoundBase* HitSound;

	UPROPERTY(EditAnywhere, Category = VisualEffects)
	UParticleSystem* HitParticles;
	
};
