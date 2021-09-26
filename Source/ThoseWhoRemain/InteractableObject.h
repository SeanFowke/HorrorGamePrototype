// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractableObject.generated.h"

UCLASS()
class THOSEWHOREMAIN_API AInteractableObject : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractableObject();
private:



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UFUNCTION()
	virtual void OnOverlapWithPlayer(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
										 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
										 bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(EditAnywhere, Category = "BoxCollision")
	class UBoxComponent* box;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UWidgetComponent* initialWidget;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UWidgetComponent* secondWidget;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* mesh;


	bool inRange = false;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	// any derived class will override this function and have the desired outcome when actually interacted with
	virtual void OnInteract();

};
