// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ElectronicDevice.generated.h"

UCLASS()
class THOSEWHOREMAIN_API AElectronicDevice : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AElectronicDevice();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	USceneComponent* root;

	UPROPERTY(EditAnywhere)
	class UAudioComponent* audioComp;

	UPROPERTY(EditAnywhere)
	class USphereComponent* sphere;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* mesh;

	UFUNCTION()
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
										 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
										 bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(EditAnywhere)
	USoundWave* normalSound;
	UPROPERTY(EditAnywhere)
	USoundWave* distortedSound;



	void PlayNormalSound();
	void PlayDistortedSound();


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
