// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "ThoseWhoRemainGameMode.h"
#include "ThoseWhoRemainHUD.h"
#include "ThoseWhoRemainCharacter.h"
#include "UObject/ConstructorHelpers.h"

AThoseWhoRemainGameMode::AThoseWhoRemainGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AThoseWhoRemainHUD::StaticClass();
}
