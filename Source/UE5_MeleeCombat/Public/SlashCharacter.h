// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "InputActionValue.h"
#include "CharacterTypes.h"
#include "SlashCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class AItem;
class UAnimMontage;



UCLASS()
class UE5_MELEECOMBAT_API ASlashCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASlashCharacter();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void Jump() override;
    FORCEINLINE	void SetOverlappingItem(AItem* Item) { OverlappingItem = Item; }
	FORCEINLINE ECharacterState GetCharacterState() const { return CharacterState; }

	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

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

	/*
	* Callbacks for input
	*/
	void IAMove(const FInputActionValue& Value);
	void IALook(const FInputActionValue& Value);
	void EKeyPressed();
  	virtual void Attack() override;
	
	/*
	* play montage functions
	*/
	virtual void PlayAttackMontage() override;
	
	virtual void AttackEnd() override;
	virtual bool CanAttack() override;

	UFUNCTION()
	void PlayEquipMontage(FName SectionName);
	bool CanDisarm();
	bool CanArm();

	UFUNCTION(BlueprintCallable)
	void Disarm();

	UFUNCTION(BlueprintCallable)
	void Arm();

	UFUNCTION(BlueprintCallable)
	void FinishEquipping();
private:
	ECharacterState CharacterState = ECharacterState::ECS_UnEquipped;

	UPROPERTY(BlueprintReadWrite,Meta = (AllowPrivateAccess="true"))
	EActionState ActionState = EActionState::EAS_Unoccupied;

	UPROPERTY(VisibleInstanceOnly)
	AItem* OverlappingItem;

	
	

	UPROPERTY(EditDefaultsOnly, Category = "Montages")
	UAnimMontage* EquipMontage;

};
