// Fill out your copyright notice in the Description page of Project Settings.


#include "Bird.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include <EnhancedInputComponent.h>
#include "EnhancedInputSubsystems.h"



// Sets default values
ABird::ABird()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//AutoPossessPlayer = EAutoReceiveInput::Player0;

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
	//playerControllerCast
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem< UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			SubSystem->AddMappingContext(BirdMappingContext, 0);
		}
	}
	
	
	
	
}

void ABird::IAMove(const FInputActionValue& value)
{
	//get input actions current value
	 const float CurrentValue = value.Get<float>();
	 if (Controller && CurrentValue != 0)
	 {
		 FVector MoveForward = GetActorForwardVector();
		 AddMovementInput(MoveForward, CurrentValue);
		
	 }
}

void ABird::IALook(const FInputActionValue& Value)
{
	const FVector2D CurrentValue = Value.Get<FVector2D>();
	if (GetController())
	{
		AddControllerYawInput(CurrentValue.X);
		AddControllerPitchInput(CurrentValue.Y);
	}
	
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

	/*PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ABird::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ABird::Turn);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &ABird::LookUp);*/
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ABird::IAMove);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ABird::IALook);
	}
}

