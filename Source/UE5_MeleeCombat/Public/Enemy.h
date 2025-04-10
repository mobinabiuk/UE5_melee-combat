// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "HitInterface.h"
#include "CharacterTypes.h"
#include "Enemy.generated.h"


class UAnimMontage;
class USoundBase;


UCLASS()
class UE5_MELEECOMBAT_API AEnemy : public ACharacter,public IHitInterface
{
	GENERATED_BODY()

public:
	AEnemy();
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void GetHit_Implementation(const FVector& ImpactPoint)override;
	void DirectionalHitReact(const FVector& ImpactPoint);
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
protected:
	virtual void BeginPlay() override;

	/*
	* play montage functions
	*/
	void PlayHitReactMontage(const FName& SectionName);
	void Die();

	UFUNCTION()
	void MoveToTarget(AActor* Target);
	AActor* ChoosePatrolTarget();
	void PatrolTimerFinished();
	bool InTargetRange(AActor* Target,double Radius);
	void CheckCombatTarget();
	void CheckPatrolTarget();
	UFUNCTION()
	void PawnSeen(APawn* SeePawn);

	UPROPERTY(BlueprintReadOnly)
	EDeathPose DeathPose = EDeathPose::EDP_Alive;

private:

	UPROPERTY(VisibleAnywhere)
	class UAttributeComponent* Attributes;

	UPROPERTY(EditAnywhere)
	class UHealthBarComponent* HealthBarWidget;

	/*
	* Anim Montages
	*/
	UPROPERTY(EditDefaultsOnly, Category = "Montages")
	UAnimMontage* HitReactMontage;

	UPROPERTY(EditAnywhere,Category=Sounds)
	USoundBase* HitSound;

	UPROPERTY(EditAnywhere, Category = VisualEffects)
	UParticleSystem* HitParticles;

	UPROPERTY(EditDefaultsOnly, Category = "Montages")
	UAnimMontage* DeathMontage;

	UPROPERTY()
	AActor* CombatTarget;

	UPROPERTY(EditAnywhere)
	double CombatRadius = 500.f;

	UPROPERTY(EditAnywhere)
	double AttackRadius = 150.f;

	/* Navigation */
	UPROPERTY()
	class AAIController* EnemyAIController;
	//Current Patrol Target
	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
	AActor* PatrolTarget;

	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
	TArray<AActor*> PatrolTargets;

	UPROPERTY(EditAnywhere)
	double PatrolRadius = 200.f;
	
	UPROPERTY()
	FTimerHandle TimerHandle;

	UPROPERTY(EditAnywhere,Category="AI Navigation")
	float WaitMin = 4.f;
	
	UPROPERTY(EditAnywhere,Category="AI Navigation")
	float WaitMax = 10.f;

	/* Components*/
	UPROPERTY(VisibleAnywhere,Category="AI Navigation")
	class UPawnSensingComponent* PawnSensingComponent;

	EEnemyState EnemyState =EEnemyState::EES_Patrolling;
};




