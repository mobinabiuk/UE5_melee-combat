// Fill out your copyright notice in the Description page of Project Settings.


#include "Bird.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/FloatingPawnMovement.h"

// Sets default values
ABird::ABird()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleRootComp"));
	RootComponent = CapsuleComp;

	BirdComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BirdComp"));
	BirdComp->SetupAttachment(GetRootComponent());

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(GetRootComponent());

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);

	FloatingPawnMovement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("MovementComponent"));

}

// Called when the game starts or when spawned
void ABird::BeginPlay()
{
	Super::BeginPlay();
	
	
}

void ABird::MoveForward(float Value)
{
	if (Controller && (Value != 0.f))
	{
		FVector ForwardDir = GetActorForwardVector();
		AddMovementInput(ForwardDir, Value);
	}
}

void ABird::Turn(float Value)
{
	
	AddControllerYawInput(Value);

	FRotator NewRotation = Controller->GetControlRotation();
	NewRotation.Pitch = GetActorRotation().Pitch;  // Keep the pitch unchanged
	SetActorRotation(NewRotation);
}

void ABird::LookUp(float Value)
{
	AddControllerPitchInput(Value);

	FRotator NewRotation = Controller->GetControlRotation();
	NewRotation.Yaw = GetActorRotation().Yaw;  // Keep the yaw unchanged
	SetActorRotation(NewRotation);
}

// Called every frame
void ABird::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABird::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ABird::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ABird::Turn);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &ABird::LookUp);

}

