


#include "BaseCharacter.h"
#include "Weapon.h"
#include "Components/BoxComponent.h"
#include "Components/AttributeComponent.h"


ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	
	Attributes = CreateDefaultSubobject<UAttributeComponent>(FName("Attributes"));

}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABaseCharacter::Attack()
{
	
}

void ABaseCharacter::PlayAttackMontage()
{
	
}

bool ABaseCharacter::CanAttack()
{
	return false;
}

void ABaseCharacter::AttackEnd()
{
	
}

void ABaseCharacter::Die()
{
	
}

void ABaseCharacter::PlayHitReactMontage(const FName& SectionName)
{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance && HitReactMontage)
		{
			AnimInstance->Montage_Play(HitReactMontage);
			AnimInstance->Montage_JumpToSection(SectionName, HitReactMontage);
		}
}

void ABaseCharacter::DirectionalHitReact(const FVector& ImpactPoint)
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


void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseCharacter::SetWeaponCollisionEnabled(ECollisionEnabled::Type CollisionEnabled)
{
	if (EquippedWeapon && EquippedWeapon->GetWeaponBox())
	{
		EquippedWeapon->GetWeaponBox()->SetCollisionEnabled(CollisionEnabled);
		EquippedWeapon->IgnoreActors.Empty();
	}
}


