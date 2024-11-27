// Fill out your copyright notice in the Description page of Project Settings.


#include "SlashCharAnimInstance.h"
#include "SlashCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void USlashCharAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	SlashCharacter = Cast<ASlashCharacter>(TryGetPawnOwner());
	if (SlashCharacter)
	{
	    SlashCharacterMovement = SlashCharacter->GetCharacterMovement();
	}
}

void USlashCharAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);
	if (SlashCharacterMovement)
	{
		//get the velocity of character
	   GroundSpeed = UKismetMathLibrary::VSizeXY(SlashCharacterMovement->Velocity);
	   IsFalling = SlashCharacterMovement->IsFalling();
	   CharacterState = SlashCharacter->GetCharacterState();
	}
}
