// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "ThoseWhoRemainHUD.h"
#include "Engine/Canvas.h"
#include "Engine/Texture2D.h"
#include "TextureResource.h"
#include "CanvasItem.h"
#include "UObject/ConstructorHelpers.h"

AThoseWhoRemainHUD::AThoseWhoRemainHUD()
{
	// Set the crosshair texture
	static ConstructorHelpers::FObjectFinder<UTexture2D> CrosshairTexObj(TEXT("/Game/FirstPerson/Textures/FirstPersonCrosshair"));
	//CrosshairTex = CrosshairTexObj.Object;
}


void AThoseWhoRemainHUD::DrawHUD()
{
	Super::DrawHUD();

}
