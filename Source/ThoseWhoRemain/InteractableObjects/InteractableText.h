// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableObject.h"

#include "InteractableText.generated.h"

/**
 * 
 */
UCLASS()
class THOSEWHOREMAIN_API AInteractableText : public AInteractableObject
{
	GENERATED_BODY()
	AInteractableText();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString text;

	UFUNCTION()
	void OnInteract() override;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UWidgetAnimation* FadeAway;

	UPROPERTY(EditAnywhere)
	float textTimerDuration;
	FTimerHandle textTimerHandle;
	void MakeTextDisapear();

	UPROPERTY(EditAnywhere)
	float fadeTimerDuration;
	FTimerHandle fadeTimerHandle;
	void RemoveTextWidget();

public:


};
