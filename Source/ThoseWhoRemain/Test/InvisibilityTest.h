// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InvisibilityTest.generated.h"

UCLASS()
class THOSEWHOREMAIN_API AInvisibilityTest : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInvisibilityTest();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	class USkeletalMeshComponent* test;

	class UHiddenObjectComponent* hiddenObjectComp;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
