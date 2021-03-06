// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "ThoseWhoRemainCharacter.h"
#include "ThoseWhoRemainProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "MotionControllerComponent.h"
#include "XRMotionControllerBase.h" // for FXRMotionControllerBase::RightHandSourceId
#include "Mirror/HiddenObjectComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "MonsterV2.h"
#include "InteractableObject.h"
#include "Components/WidgetComponent.h"
#include "Components/AudioComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);

//////////////////////////////////////////////////////////////////////////
// AThoseWhoRemainCharacter

AThoseWhoRemainCharacter::AThoseWhoRemainCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());// Position the camera
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-39.56f, 1.75f, 64.f));
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// create a hidden object component
	sceneCapture = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("SceneCapture2D"));
	sceneCapture->SetupAttachment(GetCapsuleComponent());
	sceneCapture->SetRelativeLocation(FVector(-39.56f, 1.75f, 64.f));
	
	mirrorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MirrorMesh"));
	mirrorMesh->SetupAttachment(GetCapsuleComponent());

	audioComp = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	audioComp->SetupAttachment(GetCapsuleComponent());

	widget = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));

	perceptionStimuliSource = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("AiPerceptionStimuliSource"));



	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	/*Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->RelativeRotation = FRotator(1.9f, -19.19f, 5.2f);
	Mesh1P->RelativeLocation = FVector(-0.5f, -4.4f, -155.7f);*/

	// Create a gun mesh component
	//FP_Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FP_Gun"));
	//FP_Gun->SetOnlyOwnerSee(true);			// only the owning player will see this mesh
	//FP_Gun->bCastDynamicShadow = false;
	//FP_Gun->CastShadow = false;
	//// FP_Gun->SetupAttachment(Mesh1P, TEXT("GripPoint"));
	//FP_Gun->SetupAttachment(RootComponent);

	/*FP_MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	FP_MuzzleLocation->SetupAttachment(FP_Gun);
	FP_MuzzleLocation->SetRelativeLocation(FVector(0.2f, 48.4f, -10.6f));*/

	// Default offset from the character location for projectiles to spawn
	GunOffset = FVector(100.0f, 0.0f, 10.0f);

	// Note: The ProjectileClass and the skeletal mesh/anim blueprints for Mesh1P, FP_Gun, and VR_Gun 
	// are set in the derived blueprint asset named MyCharacter to avoid direct content references in C++.

	// Create VR Controllers.
	R_MotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("R_MotionController"));
	R_MotionController->MotionSource = FXRMotionControllerBase::RightHandSourceId;
	R_MotionController->SetupAttachment(RootComponent);
	L_MotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("L_MotionController"));
	L_MotionController->SetupAttachment(RootComponent);

	// Create a gun and attach it to the right-hand VR controller.
	// Create a gun mesh component
	//VR_Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("VR_Gun"));
	//VR_Gun->SetOnlyOwnerSee(true);			// only the owning player will see this mesh
	//VR_Gun->bCastDynamicShadow = false;
	//VR_Gun->CastShadow = false;
	//VR_Gun->SetupAttachment(R_MotionController);
	//VR_Gun->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));

	/*VR_MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("VR_MuzzleLocation"));
	VR_MuzzleLocation->SetupAttachment(VR_Gun);
	VR_MuzzleLocation->SetRelativeLocation(FVector(0.000004, 53.999992, 10.000000));
	VR_MuzzleLocation->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));*/		// Counteract the rotation of the VR gun model.

	// Uncomment the following line to turn motion controllers on by default:
	//bUsingMotionControllers = true;
}

void AThoseWhoRemainCharacter::SetMonsterRef(AMonsterV2* monster_)
{
	// set monster ref
	monster = monster_;
	// go and give a ref of the player to the monster
	if (monster)
	{
		monster->SetPlayerRef(this);
	}
}

void AThoseWhoRemainCharacter::SetInteractableObjectRef(AInteractableObject* object_)
{
	objectRef = object_;
}

void AThoseWhoRemainCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	if (widget->GetUserWidgetObject() && !widget->GetUserWidgetObject()->IsInViewport())
	{
		widget->GetUserWidgetObject()->AddToViewport();
	}

	stamina = maxStamina;
	mirrorHoldAmount = mirrorHoldAmountMax;

	audioComp->Stop();

	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AThoseWhoRemainCharacter::OnOverlap);

	if (!widget->GetUserWidgetObject()->IsInViewport())
	{
		widget->GetUserWidgetObject()->AddToViewport();
	}

}

void AThoseWhoRemainCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (holdingMirror && monster)
	{
		monster->CheckIfInCamera();
	}

	
}

void AThoseWhoRemainCharacter::RemoveStamina()
{
	if (stamina > 0 && isSprinting)
	{
		stamina -= 1.0f;
		//UE_LOG(LogTemp, Warning, TEXT("Removed Stamina"));
		GetWorld()->GetTimerManager().SetTimer(sprintTimer, this, &AThoseWhoRemainCharacter::RemoveStamina, 1.0f, false);
	}
	else if(stamina <= 0)
	{
		OnWalk();
	}
}

void AThoseWhoRemainCharacter::AddStamina()
{
	if (stamina < maxStamina && !isSprinting)
	{
		stamina += 1.0f;
		//UE_LOG(LogTemp, Warning, TEXT("Added Stamina"));
		GetWorld()->GetTimerManager().SetTimer(sprintTimer, this, &AThoseWhoRemainCharacter::AddStamina, 2.0f, false);
	}
}

void AThoseWhoRemainCharacter::AddMirrorTime()
{
	if (mirrorHoldAmount < mirrorHoldAmountMax && !holdingMirror)
	{
		mirrorHoldAmount += 1.0f;
		GetWorld()->GetTimerManager().SetTimer(mirrorTimer, this, &AThoseWhoRemainCharacter::AddMirrorTime, 2.0f, false);
	}
}

void AThoseWhoRemainCharacter::RemoveMirrorTime()
{
	if (mirrorHoldAmount > 0 && holdingMirror)
	{
		mirrorHoldAmount -= 1.0f;
		//UE_LOG(LogTemp, Warning, TEXT("Removed Stamina"));
		GetWorld()->GetTimerManager().SetTimer(mirrorTimer, this, &AThoseWhoRemainCharacter::RemoveMirrorTime, 1.0f, false);
	}
	else if (mirrorHoldAmount <= 0)
	{
		OnLowerMirror();
	}
}

void AThoseWhoRemainCharacter::PlayFootstepSound()
{
	audioComp->Play();
	//UE_LOG(LogTemp, Warning, TEXT("Played Sound!"));
}

float AThoseWhoRemainCharacter::GetStaminaFillAmount()
{
	return stamina / maxStamina;
}

float AThoseWhoRemainCharacter::GetMirrorFillAmount()
{
	return mirrorHoldAmount / mirrorHoldAmountMax;
}

void AThoseWhoRemainCharacter::OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != nullptr && OtherActor != this && OtherComp != nullptr)
	{
		AMonsterV2* monster = Cast<AMonsterV2>(OtherActor);

		if (monster)
		{
			UGameplayStatics::OpenLevel(GetWorld(), "GameMap", false);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// Input

void AThoseWhoRemainCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	check(PlayerInputComponent);

	// Bind jump events
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	// Bind raise mirror event
	PlayerInputComponent->BindAction("RaiseMirror", IE_Pressed, this, &AThoseWhoRemainCharacter::OnRaiseMirror);
	PlayerInputComponent->BindAction("RaiseMirror", IE_Released, this, &AThoseWhoRemainCharacter::OnLowerMirror);

	// Bind the sprint event
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AThoseWhoRemainCharacter::OnSprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AThoseWhoRemainCharacter::OnWalk);

	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &AThoseWhoRemainCharacter::OnInteract);
	// Enable touchscreen input
	EnableTouchscreenMovement(PlayerInputComponent);

	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &AThoseWhoRemainCharacter::OnResetVR);

	// Bind movement events
	PlayerInputComponent->BindAxis("MoveForward", this, &AThoseWhoRemainCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AThoseWhoRemainCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AThoseWhoRemainCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AThoseWhoRemainCharacter::LookUpAtRate);
}

void AThoseWhoRemainCharacter::OnFire()
{
	// try and fire a projectile
	if (ProjectileClass != NULL)
	{
		UWorld* const World = GetWorld();
		if (World != NULL)
		{
			if (bUsingMotionControllers)
			{
				const FRotator SpawnRotation = VR_MuzzleLocation->GetComponentRotation();
				const FVector SpawnLocation = VR_MuzzleLocation->GetComponentLocation();
				World->SpawnActor<AThoseWhoRemainProjectile>(ProjectileClass, SpawnLocation, SpawnRotation);
			}
			else
			{
				const FRotator SpawnRotation = GetControlRotation();
				// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
				const FVector SpawnLocation = ((FP_MuzzleLocation != nullptr) ? FP_MuzzleLocation->GetComponentLocation() : GetActorLocation()) + SpawnRotation.RotateVector(GunOffset);

				//Set Spawn Collision Handling Override
				FActorSpawnParameters ActorSpawnParams;
				ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

				// spawn the projectile at the muzzle
				World->SpawnActor<AThoseWhoRemainProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
			}
		}
	}

	// try and play the sound if specified
	if (FireSound != NULL)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
	}

	// try and play a firing animation if specified
	if (FireAnimation != NULL)
	{
		// Get the animation object for the arms mesh
		/*UAnimInstance* AnimInstance = Mesh1P->GetAnimInstance();
		if (AnimInstance != NULL)
		{
			AnimInstance->Montage_Play(FireAnimation, 1.f);*/
		//}
	}
}

void AThoseWhoRemainCharacter::OnRaiseMirror()
{
	if (mirrorHoldAmount > 1)
	{
		mirrorHoldAmount -= 1.0f;
		GetWorld()->GetTimerManager().SetTimer(mirrorTimer, this, &AThoseWhoRemainCharacter::RemoveMirrorTime, 1.0f, false);
		mirrorMesh->SetRelativeLocation(FVector(4.0f, 4.305359f, 60.0f));
		mirrorMesh->SetVisibility(true);
		holdingMirror = true;
		if (monster)
		{
			monster->RaiseLight();
		}
	}
}

void AThoseWhoRemainCharacter::OnLowerMirror()
{
	mirrorMesh->SetRelativeLocation(FVector(-85.0f, 4.305359f, 60.0f));
	mirrorMesh->SetVisibility(false);
	GetWorld()->GetTimerManager().SetTimer(mirrorTimer, this, &AThoseWhoRemainCharacter::AddMirrorTime, 2.0f, false);

	holdingMirror = false;
	if (monster)
	{
		monster->DimLight();
	}
}

void AThoseWhoRemainCharacter::OnSprint()
{
	
	if (stamina > 1.0f)
	{
		stamina -= 1.0f;
		GetCharacterMovement()->MaxWalkSpeed = 600;
		isSprinting = true;
		//UE_LOG(LogTemp, Warning, TEXT("IS SPRINTING"));
		GetWorld()->GetTimerManager().SetTimer(sprintTimer, this, &AThoseWhoRemainCharacter::RemoveStamina, 1.0f, false);
	}
	else
	{
		audioComp->Stop();
	}
}

void AThoseWhoRemainCharacter::OnWalk()
{
	GetCharacterMovement()->MaxWalkSpeed = 200;
	isSprinting = false;
	//UE_LOG(LogTemp, Warning, TEXT("IS NOT SPRINTING"));
	GetWorld()->GetTimerManager().SetTimer(sprintTimer, this, &AThoseWhoRemainCharacter::AddStamina, 2.0f, false);
}

void AThoseWhoRemainCharacter::OnInteract()
{

	if (objectRef)
	{
		objectRef->OnInteract();
		UE_LOG(LogTemp, Warning, TEXT("Interacted!"));
	}
}

void AThoseWhoRemainCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AThoseWhoRemainCharacter::BeginTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if (TouchItem.bIsPressed == true)
	{
		return;
	}
	if ((FingerIndex == TouchItem.FingerIndex) && (TouchItem.bMoved == false))
	{
		OnFire();
	}
	TouchItem.bIsPressed = true;
	TouchItem.FingerIndex = FingerIndex;
	TouchItem.Location = Location;
	TouchItem.bMoved = false;
}

void AThoseWhoRemainCharacter::EndTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if (TouchItem.bIsPressed == false)
	{
		return;
	}
	TouchItem.bIsPressed = false;
}

//Commenting this section out to be consistent with FPS BP template.
//This allows the user to turn without using the right virtual joystick

//void AThoseWhoRemainCharacter::TouchUpdate(const ETouchIndex::Type FingerIndex, const FVector Location)
//{
//	if ((TouchItem.bIsPressed == true) && (TouchItem.FingerIndex == FingerIndex))
//	{
//		if (TouchItem.bIsPressed)
//		{
//			if (GetWorld() != nullptr)
//			{
//				UGameViewportClient* ViewportClient = GetWorld()->GetGameViewport();
//				if (ViewportClient != nullptr)
//				{
//					FVector MoveDelta = Location - TouchItem.Location;
//					FVector2D ScreenSize;
//					ViewportClient->GetViewportSize(ScreenSize);
//					FVector2D ScaledDelta = FVector2D(MoveDelta.X, MoveDelta.Y) / ScreenSize;
//					if (FMath::Abs(ScaledDelta.X) >= 4.0 / ScreenSize.X)
//					{
//						TouchItem.bMoved = true;
//						float Value = ScaledDelta.X * BaseTurnRate;
//						AddControllerYawInput(Value);
//					}
//					if (FMath::Abs(ScaledDelta.Y) >= 4.0 / ScreenSize.Y)
//					{
//						TouchItem.bMoved = true;
//						float Value = ScaledDelta.Y * BaseTurnRate;
//						AddControllerPitchInput(Value);
//					}
//					TouchItem.Location = Location;
//				}
//				TouchItem.Location = Location;
//			}
//		}
//	}
//}

void AThoseWhoRemainCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
		//UE_LOG(LogTemp, Warning, TEXT("Moving Forward"));	
	}

	if ((GetVelocity().X > 2.0f || GetVelocity().X < -2.0f || GetVelocity().Y > 2.0f || GetVelocity().Y < -2.0f) && GetVelocity().Z == 0)
	{
		if (!moving)
		{
			moving = true;
			GetWorld()->GetTimerManager().SetTimer(footstepTimer, this, &AThoseWhoRemainCharacter::PlayFootstepSound, footstepDelay, true);
			//UE_LOG(LogTemp, Warning, TEXT("Played Sound"));
		}
	}
	else
	{
		moving = false;
		GetWorld()->GetTimerManager().ClearTimer(footstepTimer);
		//UE_LOG(LogTemp, Warning, TEXT("Cleared Timer"));
	}

	
}

void AThoseWhoRemainCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
		//UE_LOG(LogTemp, Warning, TEXT("Moving Right"));
	}
}

void AThoseWhoRemainCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AThoseWhoRemainCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

bool AThoseWhoRemainCharacter::EnableTouchscreenMovement(class UInputComponent* PlayerInputComponent)
{
	if (FPlatformMisc::SupportsTouchInput() || GetDefault<UInputSettings>()->bUseMouseForTouch)
	{
		PlayerInputComponent->BindTouch(EInputEvent::IE_Pressed, this, &AThoseWhoRemainCharacter::BeginTouch);
		PlayerInputComponent->BindTouch(EInputEvent::IE_Released, this, &AThoseWhoRemainCharacter::EndTouch);

		//Commenting this out to be more consistent with FPS BP template.
		//PlayerInputComponent->BindTouch(EInputEvent::IE_Repeat, this, &AThoseWhoRemainCharacter::TouchUpdate);
		return true;
	}
	
	return false;
}
