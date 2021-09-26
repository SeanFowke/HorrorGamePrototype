// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterV2.h"
#include "Mirror/HiddenObjectComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Components/SpotLightComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ThoseWhoRemainCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
// Sets default values
AMonsterV2::AMonsterV2()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	root = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	root = RootComponent;

	hiddenComp = CreateDefaultSubobject<UHiddenObjectComponent>(TEXT("HiddenObjectComponent"));
	light = CreateDefaultSubobject<USpotLightComponent>(TEXT("SpotLight"));
	light->SetupAttachment(root);
}

// Called when the game starts or when spawned
void AMonsterV2::BeginPlay()
{
	Super::BeginPlay();
	isVisible = false;

	hiddenComp->Setup(GetMesh(), this);

	AThoseWhoRemainCharacter* character = Cast<AThoseWhoRemainCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (character)
	{
		character->SetMonsterRef(this);
	}

	state = Idle;

}

void AMonsterV2::CheckIfInCamera()
{

	APlayerCameraManager* camManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
	if (camManager && player)
	{
		// get the direction vector between the camera and the monster and normalize it
		FVector dir = GetActorLocation() - camManager->GetCameraLocation();
		dir.Normalize();
		// get the camera's forward
		FVector forward = camManager->GetCameraRotation().Vector();

		// if the dot product between the two is negative it means they are not facing the same way and aren't in view of the camera 
		if (FVector::DotProduct(dir, forward) < 0)
		{
			RaiseLight();
			isVisible = false;
			//UE_LOG(LogTemp, Warning, TEXT("Not visible"));
		}
		else
		{
			DimLight();
			isVisible = true;
			//UE_LOG(LogTemp, Warning, TEXT("Visible"));
		}
	}

	isVisible = false;

}

void AMonsterV2::RaiseLight()
{
	if (!isVisible)
	{
		light->SetIntensity(100000.0f);
	}
	//UE_LOG(LogTemp, Warning, TEXT("Raised Light Intensity"));

}

void AMonsterV2::DimLight()
{
	light->SetIntensity(0.0f);
	//UE_LOG(LogTemp, Warning, TEXT("Lowered Light Intensity"));
}

void AMonsterV2::ChangeSpeedToMatchState()
{
	if (state == Patrol)
	{
		GetCharacterMovement()->MaxWalkSpeed = walkSpeed;
	}
	else if (state == Attacking)
	{
		GetCharacterMovement()->MaxWalkSpeed = runSpeed;
	}
}

// Called every frame
void AMonsterV2::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMonsterV2::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AMonsterV2::SetPlayerRef(AThoseWhoRemainCharacter* character_)
{
	player = character_;
}

void AMonsterV2::ChangeState(MonsterStates state_)
{
	state = state_;

	ChangeSpeedToMatchState();
}

float AMonsterV2::GetSpeed()
{
	return GetVelocity().Size();
}

float AMonsterV2::GetAnimSpeed()
{
	if (state == Patrol)
	{
		return 1.8f;
	}
	else
	{
		return 1.0f;
	}
}

void AMonsterV2::SetMaxRunSpeed(float maxSpeed_)
{
	GetCharacterMovement()->MaxWalkSpeed = maxSpeed_;
}

float AMonsterV2::GetDirection()
{
	return GetMesh()->GetAnimInstance()->CalculateDirection(GetVelocity(), GetActorRotation());
}

MonsterStates AMonsterV2::GetState()
{
	return state;
}

