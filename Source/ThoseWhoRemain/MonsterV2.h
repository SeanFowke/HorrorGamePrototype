// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MonsterV2.generated.h"

UCLASS()
class THOSEWHOREMAIN_API AMonsterV2 : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMonsterV2();

	void CheckIfInCamera();
	void RaiseLight();
	void DimLight();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	USceneComponent* root;

	UPROPERTY(EditAnywhere)
	class UHiddenObjectComponent* hiddenComp;

	UPROPERTY(EditAnywhere)
	class USpotLightComponent* light;

	bool isVisible;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
