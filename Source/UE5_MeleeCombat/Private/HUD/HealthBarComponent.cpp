// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/HealthBarComponent.h"
#include "HUD/HealthBar.h"
#include "Components/ProgressBar.h"

void UHealthBarComponent::SetHealthPercent(float Percent)
{
	if (HealthBarWidget == nullptr)
	{
		HealthBarWidget = Cast<UHealthBar>(GetUserWidgetObject());
	}
    if (IsValid(HealthBarWidget),IsValid(HealthBarWidget->HealthBar))
    {
	   HealthBarWidget->HealthBar->SetPercent(Percent);
    }
}
