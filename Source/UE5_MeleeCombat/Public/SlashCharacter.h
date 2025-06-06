// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "InputActionValue.h"
#include "CharacterTypes.h"
#include "PickupInterface.h"
#include "SlashCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class AItem;
class ASoul;
class ATreasure;
class UAnimMontage;
class USlashOverlay;

UCLASS()
class UE5_MELEECOMBAT_API ASlashCharacter : public ABaseCharacter,public IPickupInterface
{
	GENERATED_BODY()

public:
	ASlashCharacter();
	virtual void Tick(float DeltaSeconds) override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void Jump() override;
	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter)override;
	virtual void SetOverlappingItem(AItem* Item) override;
	virtual void AddSouls(ASoul* Soul) override;
	virtual void AddGold(ATreasure* Treasure) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;


protected:
	virtual void BeginPlay() override;
	
    /** Callbacks for input*/
	void IAMove(const FInputActionValue& Value);
	void IALook(const FInputActionValue& Value);
	void Dodge();
	void EKeyPressed();
  	virtual void Attack() override;
	
	/** Combat*/
	void EquipWeapon(AWeapon* Weapon);
	virtual void AttackEnd() override;
	virtual void DodgeEnd() override;
	virtual bool CanAttack() override;
	bool CanArm();
	bool CanDisarm();
	virtual void Die() override;
	bool IsOccupied();
	bool HasEnoughStamina();

	UFUNCTION()
	void PlayEquipMontage(FName SectionName);
	
	UFUNCTION(BlueprintCallable)
	void AttachWeaponToBack();

	UFUNCTION(BlueprintCallable)
	void AttachWeaponToHand();

	UFUNCTION(BlueprintCallable)
	void FinishEquipping();

	UFUNCTION(BlueprintCallable)
	void HitReactEnd();
	
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArmComp;

    UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "EnhancedInput")
	UInputMappingContext* CharMappingContext;

	UPROPERTY(EditAnyWhere,BlueprintReadOnly,Category="EnhancedInput")
	UInputAction* MoveAction;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "EnhancedInput")
	UInputAction* LookAction;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "EnhancedInput")
	UInputAction* JumpAction;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "EnhancedInput")
	UInputAction* EquipAction;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "EnhancedInput")
	UInputAction* AttackAction;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "EnhancedInput")
	UInputAction* DodgeAction;
	
	UPROPERTY(EditDefaultsOnly, Category = "Montages")
	UAnimMontage* EquipMontage;

	UPROPERTY(VisibleInstanceOnly)
	AItem* OverlappingItem;
	
private:
	
	void InitializeSlashOverlay();
	void SetHUDHealth();
	bool IsUnoccupied();
	
	ECharacterState CharacterState = ECharacterState::ECS_UnEquipped;

	UPROPERTY(BlueprintReadWrite,Meta = (AllowPrivateAccess="true"))
	EActionState ActionState = EActionState::EAS_Unoccupied;

	UPROPERTY()
	USlashOverlay* SlashOverlay;

public:
	FORCEINLINE ECharacterState GetCharacterState() const { return CharacterState; }
	FORCEINLINE EActionState GetActionState() const { return ActionState; }
	
};

