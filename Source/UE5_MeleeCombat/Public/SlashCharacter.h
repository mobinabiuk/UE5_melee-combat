// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "SlashCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class AItem;

UCLASS()
class UE5_MELEECOMBAT_API ASlashCharacter : public ACharacter
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
	UInputAction* Equip;

	

	void IAMove(const FInputActionValue& Value);
	void IALook(const FInputActionValue& Value);
	void EKeyPressed();
	
private:
	UPROPERTY(VisibleInstanceOnly)
	AItem* OverlappingItem;

};
