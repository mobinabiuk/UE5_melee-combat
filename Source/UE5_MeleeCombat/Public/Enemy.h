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
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	virtual void Destroyed() override;
	
    virtual void GetHit_Implementation(const FVector& ImpactPoint)override;

protected:
	virtual void BeginPlay() override;
	
	/*
	* play montage functions
	*/
	
	virtual void Die() override;
	virtual void HandleDamage(float DamageAmount) override;
	virtual void AttackEnd() override;
	virtual void Attack() override;
	virtual bool CanAttack() override;
	virtual int32 PlayDeathMontage() override;

	UPROPERTY(BlueprintReadOnly)
	TEnumAsByte<EDeathPose> DeathPose;
	
	UPROPERTY(BlueprintReadOnly)
	EEnemyState EnemyState =EEnemyState::EES_Patrolling;

	
private:

    /** AI Behavior */
	void InitializeEnemy();
    void CheckPatrolTarget();
    void CheckCombatTarget();
	void PatrolTimerFinished();
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
    bool InTargetRange(AActor* Target,double Radius);
    UFUNCTION()
    void MoveToTarget(AActor* Target);
    AActor* ChoosePatrolTarget();
	void SpawnDefaultWeapon();
	
	UFUNCTION()
	void PawnSeen(APawn* SeenPawn);

	
	UPROPERTY(EditAnywhere)
	class UHealthBarComponent* HealthBarWidget;

	UPROPERTY(VisibleAnywhere,Category="AI Navigation")
	class UPawnSensingComponent* PawnSensingComponent;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AWeapon> WeaponClass;
	
	UPROPERTY(EditAnywhere, Category = Combat)
	double CombatRadius = 600.f;

	UPROPERTY(EditAnywhere, Category = Combat)
	double AttackRadius = 150.f;

	UPROPERTY(EditAnywhere, Category = Combat)
	double AcceptanceRadius = 40.f;

	UPROPERTY()
	class AAIController* EnemyAIController;

	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
	AActor* PatrolTarget;

	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
	TArray<AActor*> PatrolTargets;

	UPROPERTY(EditAnywhere)
	double PatrolRadius = 180.f;

	UPROPERTY()
	FTimerHandle PatrolTimer;

	UPROPERTY(EditAnywhere,Category="AI Navigation")
	float PatrolWaitMin = 4.f;
	
	UPROPERTY(EditAnywhere,Category="AI Navigation")
	float PatrolWaitMax = 10.f;
	
	UPROPERTY(EditAnywhere, Category = Combat)
	float PatrollingSpeed = 125.f;

	FTimerHandle AttackTimer;

	UPROPERTY(EditAnywhere, Category = Combat)
	float AttackMin = 0.5f;

	UPROPERTY(EditAnywhere, Category = Combat)
	float AttackMax = 1.f;

	UPROPERTY(EditAnywhere, Category = Combat)
	float ChasingSpeed = 300.f;

	UPROPERTY(EditAnywhere, Category=Combat)
	float DeathLifeSpan =  7.f;
	
	UPROPERTY()
	AActor* CombatTarget;
	
	
};






