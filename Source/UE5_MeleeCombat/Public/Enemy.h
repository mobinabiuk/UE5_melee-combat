// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "CharacterTypes.h"
#include "Enemy.generated.h"


class USoundBase;


UCLASS()
class UE5_MELEECOMBAT_API AEnemy : public ABaseCharacter
{
	GENERATED_BODY()

public:
	AEnemy();
	virtual void Tick(float DeltaTime) override;
	virtual void GetHit_Implementation(const FVector& ImpactPoint)override;
	
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
protected:
	virtual void BeginPlay() override;

	/*
	* play montage functions
	*/
	
	virtual void Die() override;
	virtual void HandleDamage(float DamageAmount) override;

	UFUNCTION()
	void MoveToTarget(AActor* Target);
	AActor* ChoosePatrolTarget();
	void PatrolTimerFinished();
	bool InTargetRange(AActor* Target,double Radius);
	void CheckCombatTarget();
	void CheckPatrolTarget();
	virtual void Destroyed() override;
	virtual int32 PlayDeathMontage() override;

	UPROPERTY(EditAnywhere, Category=Combat)
	float DeathLifeSpan =  7.f;
	
	UFUNCTION()
	void PawnSeen(APawn* SeenPawn);

	UPROPERTY(BlueprintReadOnly)
	TEnumAsByte<EDeathPose> DeathPose;
	
	UPROPERTY(BlueprintReadOnly)
	EEnemyState EnemyState =EEnemyState::EES_Patrolling;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AWeapon> WeaponClass;

	virtual void Attack() override;
	virtual bool CanAttack() override;

private:
	
	UPROPERTY(EditAnywhere)
	class UHealthBarComponent* HealthBarWidget;

	/*
	* Anim Montages
	*/
	

	UPROPERTY()
	AActor* CombatTarget;

	UPROPERTY(EditAnywhere)
	double CombatRadius = 600.f;

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
	double PatrolRadius = 180.f;

	UPROPERTY(EditAnywhere, Category = Combat)
	float PatrollingSpeed = 125.f;
	
	UPROPERTY(EditAnywhere, Category = Combat)
	float ChasingSpeed = 300.f;
	
	UPROPERTY()
	FTimerHandle PatrolTimer;

	UPROPERTY(EditAnywhere,Category="AI Navigation")
	float WaitMin = 4.f;
	
	UPROPERTY(EditAnywhere,Category="AI Navigation")
	float WaitMax = 10.f;

	/* Components*/
	UPROPERTY(VisibleAnywhere,Category="AI Navigation")
	class UPawnSensingComponent* PawnSensingComponent;

	/** AI Behavior */

	void HideHealthBar();
	void ShowHealthBar();
	void LoseInterest();
	void StartPatrolling();
	void ChaseTarget();
	bool IsOutsideCombatRadius();
	bool IsOutsideAttackRadius();
	bool IsInsideAttackRadius();
	bool IsChasing();
	bool IsAttacking();
	bool IsDead();
	bool IsEngaged();
	void ClearPatrolTimer();
	void StartAttackTimer();
	void ClearAttackTimer();
	

	/*Combat*/
	
	FTimerHandle AttackTimer;

	UPROPERTY(EditAnywhere, Category = Combat)
	float AttackMin = 0.5f;

	UPROPERTY(EditAnywhere, Category = Combat)
	float AttackMax = 1.f;
	
};






