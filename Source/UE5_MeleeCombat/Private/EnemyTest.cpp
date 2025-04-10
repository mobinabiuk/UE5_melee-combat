// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyTest.h"

#include "AIController.h"
#include "AITypes.h"
#include "Navigation/PathFollowingComponent.h"

// Sets default values
AEnemyTest::AEnemyTest()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEnemyTest::BeginPlay()
{
	Super::BeginPlay();
	
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

// Called every frame
void AEnemyTest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemyTest::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

