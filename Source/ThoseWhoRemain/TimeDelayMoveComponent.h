// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TimeDelayMoveComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class THOSEWHOREMAIN_API UTimeDelayMoveComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTimeDelayMoveComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// movement of the floor
	UPROPERTY(EditAnywhere)
	float delay;

	UPROPERTY(EditAnywhere)
	AActor* moveLoc;

	bool shouldMove = false;

	UPROPERTY(EditAnywhere)
	float movementSpeed;
	// sounds
	UPROPERTY(EditAnywhere)
	class UAudioComponent* audioComp;

	// functions
	void MoveAfterDelay();
	void HandleMovement();
	// timer
	FTimerHandle timerHandle;

	UPROPERTY(EditAnywhere)
	AActor* floorToDelete;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
