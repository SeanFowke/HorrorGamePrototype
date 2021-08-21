// Fill out your copyright notice in the Description page of Project Settings.


#include "InvisibilityTest.h"
#include "Components/SkeletalMeshComponent.h"
#include "Mirror/HiddenObjectComponent.h"

// Sets default values
AInvisibilityTest::AInvisibilityTest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	hiddenObjectComp = CreateDefaultSubobject<UHiddenObjectComponent>(TEXT("Hidden Object Component"));

	test = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	test->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AInvisibilityTest::BeginPlay()
{
	Super::BeginPlay();
	
	hiddenObjectComp->Setup(test, this);
}

// Called every frame
void AInvisibilityTest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

