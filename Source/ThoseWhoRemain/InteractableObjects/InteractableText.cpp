// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableObjects/InteractableText.h"
#include "Components/WidgetComponent.h"
#include "ThoseWhoRemainCharacter.h"
#include "Animation/WidgetAnimation.h"


AInteractableText::AInteractableText()
{

}

void AInteractableText::OnInteract()
{
	// remove the initial widget
	if (initialWidget && initialWidget->GetUserWidgetObject() && initialWidget->GetUserWidgetObject()->IsInViewport())
	{
		initialWidget->GetUserWidgetObject()->RemoveFromViewport();
	}
	// replace it with the text widget
	if (secondWidget && secondWidget->GetUserWidgetObject() && !secondWidget->GetUserWidgetObject()->IsInViewport())
	{
		secondWidget->GetUserWidgetObject()->AddToViewport();
		GetWorld()->GetTimerManager().SetTimer(textTimerHandle, this, &AInteractableText::MakeTextDisapear, textTimerDuration, false);
	}
}

void AInteractableText::MakeTextDisapear()
{
	if (FadeAway)
	{
		
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("FadeAway is NULL"));
	}

	if (secondWidget && secondWidget->GetUserWidgetObject())
	{
		secondWidget->GetUserWidgetObject()->PlayAnimation(FadeAway);
		GetWorld()->GetTimerManager().SetTimer(fadeTimerHandle, this, &AInteractableText::RemoveTextWidget, fadeTimerDuration, false);
	}
	
}

void AInteractableText::RemoveTextWidget()
{
	if (secondWidget && secondWidget->GetUserWidgetObject() && secondWidget->GetUserWidgetObject()->IsInViewport())
	{
		secondWidget->GetUserWidgetObject()->RemoveFromViewport();
	}
}
