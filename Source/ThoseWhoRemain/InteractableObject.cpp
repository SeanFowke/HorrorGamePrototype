// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableObject.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"
#include "ThoseWhoRemainCharacter.h"
#include "Components/WidgetComponent.h"

// Sets default values
AInteractableObject::AInteractableObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	//root = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));


	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	RootComponent = mesh;
	//mesh->SetupAttachment();

	box = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	box->SetupAttachment(mesh);
	box->SetCollisionProfileName("OverlapAllDynamic");
	
	initialWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));
	secondWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent2"));


}

// Called when the game starts or when spawned
void AInteractableObject::BeginPlay()
{
	Super::BeginPlay();

	box->OnComponentBeginOverlap.AddDynamic(this, &AInteractableObject::OnOverlapWithPlayer);
	box->OnComponentEndOverlap.AddDynamic(this, &AInteractableObject::OnOverlapEnd);


}

void AInteractableObject::OnOverlapWithPlayer(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != nullptr && OtherActor != this && OverlappedComp != nullptr)
	{
		// first step is to cast to the player
		AThoseWhoRemainCharacter* player = Cast<AThoseWhoRemainCharacter>(OtherActor);
		
		if (player)
		{
			//UE_LOG(LogTemp, Warning, TEXT("Set Interact Ref"));
			player->SetInteractableObjectRef(this);

			if (initialWidget && initialWidget->GetUserWidgetObject() &&!initialWidget->GetUserWidgetObject()->IsInViewport() && !secondWidget->GetUserWidgetObject()->IsInViewport())
			{
				initialWidget->GetUserWidgetObject()->AddToViewport();
			}
		}
	}
}

void AInteractableObject::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor != nullptr && OtherActor != this && OverlappedComp != nullptr)
	{
		// first step is to cast to the player
		AThoseWhoRemainCharacter* player = Cast<AThoseWhoRemainCharacter>(OtherActor);

		if (player)
		{
			player->SetInteractableObjectRef(nullptr);

			if (initialWidget && initialWidget->GetUserWidgetObject() && initialWidget->GetUserWidgetObject()->IsInViewport())
			{
				initialWidget->GetUserWidgetObject()->RemoveFromViewport();
			}
		}
	}
}

void AInteractableObject::OnInteract()
{
	UE_LOG(LogTemp, Warning, TEXT("Interact is Working!"));

	if (initialWidget && initialWidget->GetUserWidgetObject() && initialWidget->GetUserWidgetObject()->IsInViewport())
	{
		initialWidget->GetUserWidgetObject()->RemoveFromViewport();
	}
}

// Called every frame
void AInteractableObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

