// Fill out your copyright notice in the Description page of Project Settings.


#include "TimeDelayMoveComponent.h"
#include "Components/AudioComponent.h"

// Sets default values for this component's properties
UTimeDelayMoveComponent::UTimeDelayMoveComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;


	audioComp = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));

	// ...
}


// Called when the game starts
void UTimeDelayMoveComponent::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(timerHandle, this, &UTimeDelayMoveComponent::MoveAfterDelay, delay, false);
	audioComp->Stop();

	// ...
	
}


void UTimeDelayMoveComponent::MoveAfterDelay()
{
	shouldMove = true;

	if (audioComp)
	{
		audioComp->Play();
	}
}

void UTimeDelayMoveComponent::HandleMovement()
{
	if (floorToDelete)
	{
		floorToDelete->Destroy();
		shouldMove = false;
		/*FVector currentLoc = floorToDelete->GetActorLocation();
		FVector dir = moveLoc->GetActorLocation() - currentLoc;
		dir.Normalize();
		floorToDelete->SetActorLocation(currentLoc + (dir * movementSpeed));

		if (FVector::Dist(moveLoc->GetActorLocation(), currentLoc) <= 5.0f)
		{
			shouldMove = false;
		}*/
	}
}

// Called every frame
void UTimeDelayMoveComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (shouldMove)
	{
		HandleMovement();
	}
	// ...
}

