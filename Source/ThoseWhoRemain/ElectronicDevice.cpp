// Fill out your copyright notice in the Description page of Project Settings.


#include "ElectronicDevice.h"
#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"
#include "MonsterV2.h"

// Sets default values
AElectronicDevice::AElectronicDevice()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	root = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	RootComponent = root;

	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	mesh->SetupAttachment(root);

	sphere = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	sphere->SetupAttachment(root);
	sphere->SetCollisionProfileName("OverlapAllDynamic");

	audioComp = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	audioComp->SetupAttachment(root);
	

}

// Called when the game starts or when spawned
void AElectronicDevice::BeginPlay()
{
	Super::BeginPlay();

	sphere->OnComponentBeginOverlap.AddDynamic(this, &AElectronicDevice::OnOverlapBegin);
	sphere->OnComponentEndOverlap.AddDynamic(this, &AElectronicDevice::OnOverlapEnd);

	PlayNormalSound();
	
}

void AElectronicDevice::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != nullptr && OtherActor != this && OverlappedComp != nullptr)
	{
		// cast it to the monster
		AMonsterV2* monster = Cast<AMonsterV2>(OtherActor);

		if (monster)
		{
			// go play the distorted sound
			PlayDistortedSound();
		}
	}
}

void AElectronicDevice::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor != nullptr && OtherActor != this && OverlappedComp != nullptr)
	{
		// same process but go back to playing the regular song
		AMonsterV2* monster = Cast<AMonsterV2>(OtherActor);

		if (monster)
		{
			PlayNormalSound();
		}
	}
}

void AElectronicDevice::PlayNormalSound()
{
	if (normalSound)
	{
		audioComp->Sound = normalSound;
		audioComp->FadeIn(1.0f);
	}

}

void AElectronicDevice::PlayDistortedSound()
{
	if (distortedSound)
	{
		audioComp->Sound = distortedSound;
		audioComp->FadeIn(1.0f);
	}
}

// Called every frame
void AElectronicDevice::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

