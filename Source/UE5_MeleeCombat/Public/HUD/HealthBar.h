// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "HealthBar.generated.h"

/**
 * 
 */
UCLASS()
class UE5_MELEECOMBAT_API UHealthBar : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	UProgressBar* HealthBar;
};
