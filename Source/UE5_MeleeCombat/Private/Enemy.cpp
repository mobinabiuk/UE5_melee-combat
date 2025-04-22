
#include "Enemy.h"

#include "SlashCharacter.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/AttributeComponent.h"
#include "HUD/HealthBarComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Runtime/AIModule/Classes/AIController.h"
#include "TimerManager.h"
#include "Perception/PawnSensingComponent.h"
#include "Weapon.h"
#include "Navigation/PathFollowingComponent.h"


AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;
	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	
	//Generating Overlap Events
	GetMesh()->SetGenerateOverlapEvents(true);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

	

	HealthBarWidget=CreateDefaultSubobject<UHealthBarComponent>(FName("HealthBar"));
	HealthBarWidget->SetupAttachment(GetRootComponent());
	GetCharacterMovement()->bOrientRotationToMovement=true;
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>(FName("PawnSensingComponent"));
	PawnSensingComponent->SightRadius = 4000.f;
	PawnSensingComponent->SetPeripheralVisionAngle(45.f);

}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	if (IsValid(HealthBarWidget))
	{
		HealthBarWidget->SetVisibility(false);
	}
	
	EnemyAIController = Cast<AAIController>(GetController());
	GetWorldTimerManager().SetTimer(PatrolTimer,this,&AEnemy::PatrolTimerFinished,2.f);
	if (IsValid(PawnSensingComponent))
	{
		PawnSensingComponent->OnSeePawn.AddDynamic(this,&AEnemy::PawnSeen);
	}

	UWorld* World = GetWorld();
	if (IsValid(WeaponClass)&&IsValid(World))
	{
	   AWeapon* DefaultWeapon =World->SpawnActor<AWeapon>(WeaponClass);
		DefaultWeapon->Equip(GetMesh(),FName("RightHandSocket"),this,this);
		EquippedWeapon = DefaultWeapon;
	   if (IsValid(DefaultWeapon))
	   {
	   	FVector NewScale = FVector(1.f, 1.f, 1.f); // Scale all axes to 1.5
	   	DefaultWeapon->SetActorScale3D(NewScale);
	   }
	}
}


void AEnemy::Die()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && DeathMontage)
	{
		AnimInstance->Montage_Play(DeathMontage);
		int32 Selection = FMath::RandRange(0, 2);
		FName SectionName = FName();
		switch (Selection)
		{
		case 0:
			SectionName = FName("Death1");
			DeathPose = EDeathPose::EDP_Death1;
			break;

		case 1:
			SectionName = FName("Death2");
			DeathPose = EDeathPose::EDP_Death2;
			break;

		case 2:
			SectionName = FName("Death3");
			DeathPose = EDeathPose::EDP_Death3;
			break;

		default:
			break;
		}
		AnimInstance->Montage_JumpToSection(SectionName, DeathMontage);

		if (IsValid(HealthBarWidget))
		{
			HealthBarWidget->SetVisibility(false);
		}

		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		SetLifeSpan(5.f);
	}
}

void AEnemy::Attack()
{
	Super::Attack();
	PlayAttackMontage();
}

bool AEnemy::CanAttack()
{
	bool bCanAttack =
		IsInsideAttackRadius() &&
		!IsAttacking() &&
		!IsEngaged() &&
		!IsDead();
	return bCanAttack;
}

void AEnemy::HideHealthBar()
{
	if (HealthBarWidget)
	{
		HealthBarWidget->SetVisibility(false);
	}
}

void AEnemy::ShowHealthBar()
{
	if (HealthBarWidget)
	{
		HealthBarWidget->SetVisibility(true);
	}
}

void AEnemy::LoseInterest()
{
	CombatTarget = nullptr;
	HideHealthBar();
}

void AEnemy::StartPatrolling()
{
	EnemyState = EEnemyState::EES_Patrolling;
	GetCharacterMovement()->MaxWalkSpeed = 150.f;
	MoveToTarget(PatrolTarget);
}

void AEnemy::ChaseTarget()
{
	EnemyState = EEnemyState::EES_Chasing;
	GetCharacterMovement()->MaxWalkSpeed = ChasingSpeed;
	MoveToTarget(CombatTarget);
}

bool AEnemy::IsOutsideCombatRadius()
{
	return !InTargetRange(CombatTarget, CombatRadius);
}

bool AEnemy::IsOutsideAttackRadius()
{
	return !InTargetRange(CombatTarget, AttackRadius);
}

bool AEnemy::IsInsideAttackRadius()
{
	return InTargetRange(CombatTarget, AttackRadius);
}

bool AEnemy::IsChasing()
{
	return EnemyState == EEnemyState::EES_Chasing;
}

bool AEnemy::IsAttacking()
{
	return EnemyState == EEnemyState::EES_Attacking;
}

bool AEnemy::IsDead()
{
	return EnemyState == EEnemyState::EES_Dead;
}

bool AEnemy::IsEngaged()
{
	return EnemyState == EEnemyState::EES_EngagedAttacking;
}

void AEnemy::ClearPatrolTimer()
{
	GetWorldTimerManager().ClearTimer(PatrolTimer);
}

void AEnemy::ClearAttackTimer()
{
	GetWorldTimerManager().ClearTimer(AttackTimer);
}

void AEnemy::StartAttackTimer()
{
	EnemyState = EEnemyState::EES_Attacking;
	const float AttackTime = FMath::RandRange(AttackMin, AttackMax);
	GetWorldTimerManager().SetTimer(AttackTimer, this, &AEnemy::Attack, AttackTime);
}


void AEnemy::PlayAttackMontage()
{
	Super::PlayAttackMontage();

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && AttackMontage)
	{
		if (!AnimInstance->Montage_IsPlaying(AttackMontage))
		{
			AnimInstance->Montage_Play(AttackMontage);
			int32 Selection = FMath::RandRange(0, 2);
			FName SectionName = FName();
			switch (Selection)
			{
			case 0:
				SectionName = FName("Attack1");
				break;

			case 1:
				SectionName = FName("Attack2");
				break;

			case 2:
				SectionName = FName("Attack3");
				break;

			default:
				break;
			}
			AnimInstance->Montage_JumpToSection(SectionName, AttackMontage);
		}
	}
}

void AEnemy::HandleDamage(float DamageAmount)
{
	Super::HandleDamage(DamageAmount);
	if (Attributes && HealthBarWidget)
	{
		HealthBarWidget->SetHealthPercent(Attributes->GetHealthPercent());
	}
}

void AEnemy::MoveToTarget(AActor* Target)
{
	if (EnemyAIController == nullptr || Target == nullptr) return;
	FAIMoveRequest MoveRequest;
	MoveRequest.SetGoalActor(Target);
	MoveRequest.SetAcceptanceRadius(15.f);
	EnemyAIController->MoveTo(MoveRequest);
}

void AEnemy::PatrolTimerFinished()
{
	MoveToTarget(PatrolTarget);
}

bool AEnemy::InTargetRange(AActor* Target,double Radius)
{
	if (Target == nullptr) return false;
	const double DistanceToTarget = (Target->GetActorLocation()-GetActorLocation()).Size();
	return DistanceToTarget <= Radius;
}


void AEnemy::CheckCombatTarget()
{
	if (IsOutsideCombatRadius())
	{
		//outside combat radius lose interest
		ClearAttackTimer();
		LoseInterest();
		if (!IsEngaged()) StartPatrolling();
	}
	else if (IsOutsideAttackRadius() && !IsChasing())
	{
		//outside attack range
		ClearAttackTimer();
		if (!IsEngaged()) ChaseTarget();
	}
	else if (CanAttack())
	{
		//inside attack range
		StartAttackTimer();
	}
}

void AEnemy::CheckPatrolTarget()
{
	if (InTargetRange(PatrolTarget,PatrolRadius))
	{
		PatrolTarget = ChoosePatrolTarget();
		const float WaitTime = FMath::RandRange(WaitMin,WaitMax);
		GetWorldTimerManager().SetTimer(PatrolTimer,this,&AEnemy::PatrolTimerFinished,WaitTime);
	}
}

void AEnemy::Destroyed()
{
	if (IsValid(EquippedWeapon))
	{
		EquippedWeapon->Destroy();
	}
}

void AEnemy::PawnSeen(APawn* SeenPawn)
{
	const bool bShouldChaseTarget =
		EnemyState != EEnemyState::EES_Dead &&
		EnemyState != EEnemyState::EES_Chasing &&
		EnemyState < EEnemyState::EES_Attacking &&
		SeenPawn->ActorHasTag(FName("SlashCharacter"));

	if (bShouldChaseTarget)
	{
		CombatTarget = SeenPawn;
		ClearPatrolTimer();
		ChaseTarget();
	}
}

void AEnemy::Tick(float DeltaTime)
{
	if (IsDead()) return;
	Super::Tick(DeltaTime);
	if (EnemyState>EEnemyState::EES_Patrolling)
	{
		CheckCombatTarget();
	}
	else
	{
		CheckPatrolTarget();
	}
}

AActor* AEnemy::ChoosePatrolTarget()
{
	TArray<AActor*> ValidTargets;
	for (AActor* Target : PatrolTargets)
	{
		if (Target !=PatrolTarget)
		{
			ValidTargets.AddUnique(Target);
		}
	}
	const int32 NumPatrolTargets = ValidTargets.Num();
	if (NumPatrolTargets > 0)
	{
		const int32 TargetSelection = FMath::RandRange(0, NumPatrolTargets - 1);
		return ValidTargets[TargetSelection];
	}
	return nullptr;
}


void AEnemy::GetHit_Implementation(const FVector& ImpactPoint)
{
	if (!IsDead()) ShowHealthBar();
	
	if (IsAlive())
	{
		DirectionalHitReact(ImpactPoint);
	}
	else Die();
	PlayHitSound(ImpactPoint);
	SpawnHitParticles(ImpactPoint);
}


float AEnemy::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator, AActor* DamageCauser)
{
	HandleDamage(DamageAmount);
	CombatTarget = EventInstigator->GetPawn();
	
	if (IsInsideAttackRadius())
	{
		EnemyState = EEnemyState::EES_Attacking;
	}
	else if (IsOutsideAttackRadius())
	{
		ChaseTarget();
	}

	return DamageAmount;
}

