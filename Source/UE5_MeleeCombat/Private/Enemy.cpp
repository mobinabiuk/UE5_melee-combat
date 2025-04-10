
#include "Enemy.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/AttributeComponent.h"
#include "HUD/HealthBarComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Navigation/PathFollowingComponent.h"
#include "Runtime/AIModule/Classes/AIController.h"
#include "TimerManager.h"


AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;
	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	
	//Generating Overlap Events
	GetMesh()->SetGenerateOverlapEvents(true);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

	Attributes = CreateDefaultSubobject<UAttributeComponent>(FName("Attributes"));

	HealthBarWidget=CreateDefaultSubobject<UHealthBarComponent>(FName("HealthBar"));
	HealthBarWidget->SetupAttachment(GetRootComponent());
	GetCharacterMovement()->bOrientRotationToMovement=true;
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	

}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	if (IsValid(HealthBarWidget))
	{
		HealthBarWidget->SetVisibility(false);
	}

	GetWorldTimerManager().SetTimer(
		TimerHandle, // Timer handle, can also store in a member variable if needed
		this,           // Object to call the function on
		&AEnemy::OnTimerFinished, // Function to call
		2.0f,           // Delay in seconds
		false           // Do not loop
	);
	
}

void AEnemy::PlayHitReactMontage(const FName& SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && HitReactMontage)
	{
		AnimInstance->Montage_Play(HitReactMontage);
		AnimInstance->Montage_JumpToSection(SectionName, HitReactMontage);
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

void AEnemy::OnTimerFinished()
{
	EnemyAIController = Cast<AAIController>(GetController());
	if (IsValid(EnemyAIController) && IsValid(PatrolTarget))
	{
		FAIMoveRequest MoveRequest;
		MoveRequest.SetGoalActor(PatrolTarget);
		FVector PatrolLocation = PatrolTarget->GetActorLocation();
		UE_LOG(LogTemp, Warning, TEXT("Target Point Location Set to: %s"), *PatrolLocation.ToString());
		MoveRequest.SetAcceptanceRadius(15.f);
		FNavPathSharedPtr NavPath;
		EnemyAIController->MoveTo(MoveRequest, &NavPath);
		if (NavPath.IsValid())
		{
			TArray<FNavPathPoint>& PathPoints = NavPath->GetPathPoints();
			for (auto& Point : PathPoints)
			{
				const FVector& Location = Point.Location;
				DrawDebugSphere(GetWorld(),Location,12.f,12,FColor::Green,false,10.f);
			
			}
		}
	}
}


void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (IsValid(CombatTarget))
	{
	    const double DistanceToTarget = (CombatTarget->GetActorLocation()-GetActorLocation()).Size();
		if (DistanceToTarget>CombatRadius)
		{
			CombatTarget = nullptr;
			if (IsValid(HealthBarWidget))
			{
				HealthBarWidget->SetVisibility(false);
			}
		}
	}

}

void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemy::GetHit_Implementation(const FVector& ImpactPoint)
{
	if (IsValid(HealthBarWidget))
	{
		HealthBarWidget->SetVisibility(true);
	}
	
	if (IsValid(Attributes),Attributes->IsAlive())
	{
		DirectionalHitReact(ImpactPoint);
	}
	else
	{
		Die();
	}
	
	if (HitSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, HitSound, ImpactPoint);

	}
	if (HitParticles)
	{
		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			HitParticles,
			ImpactPoint
		);
	}
}

void AEnemy::DirectionalHitReact(const FVector& ImpactPoint)
{
	//dot product
	const FVector Forward = GetActorForwardVector();
	//lower impact point to the enemys actor location z
	const FVector ImpactLowered(ImpactPoint.X, ImpactPoint.Y, GetActorLocation().Z);
	const FVector ToHit = (ImpactPoint - GetActorLocation()).GetSafeNormal();
	//Forward*ToHit=|Forward||ToHit|*Cos(Theta)
	//|Forward|=1,|ToHit|=1 so Forward*ToHit=Cos(Theta)
	const double CosTheta = FVector::DotProduct(Forward, ToHit);
	// (arc-cos(theta)) to get the theta
	double Theta = FMath::Acos(CosTheta);
	//convert from radiance to degrees
	Theta = FMath::RadiansToDegrees(Theta);

	//if cross product points down theta should be negative
	const FVector CrossProduct = FVector::CrossProduct(Forward, ToHit);
	if (CrossProduct.Z<0)
	{
		Theta *= -1.f;
	}
	//play suitable react montage
	FName Section("FromBack");
	if (Theta >= -45.f && Theta<45.f)
	{
		Section = FName("FromFront");
	}
	else if (Theta >= -135.f && Theta < -45.f)
	{
		Section = FName("FromLeft");
	}
	else if (Theta >= 45.f && Theta<135.f)
	{
		Section = FName("FromRight");
	}

	PlayHitReactMontage(Section);

}

float AEnemy::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator, AActor* DamageCauser)
{
	//return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	if (IsValid(Attributes),IsValid(HealthBarWidget))
	{
		Attributes->ReceiveDamage(DamageAmount);
		HealthBarWidget->SetHealthPercent(Attributes->GetHealthPercent());
	}
  	 CombatTarget = EventInstigator->GetPawn();
	return DamageAmount;
}

