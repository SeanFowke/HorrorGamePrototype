// Fill out your copyright notice in the Description page of Project Settings.


#include "HiddenObjectComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"

// Sets default values for this component's properties
UHiddenObjectComponent::UHiddenObjectComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}


// Called when the game starts
void UHiddenObjectComponent::BeginPlay()
{
	Super::BeginPlay();
	// ...
	
}


// Called every frame
void UHiddenObjectComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


void UHiddenObjectComponent::Setup(USkeletalMeshComponent* mesh_, AActor* actor_)
{
	// first set up the mesh pointer
	if (mesh_)
	{
		mesh_->SetOwnerNoSee(true);
	}
	// then set up the actor pointer
	if (actor_)
	{
		actor_->SetOwner(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		actor_->GetOwner();
	}

}


