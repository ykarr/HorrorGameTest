// Copyright Epic Games, Inc. All Rights Reserved.

#include "Horror28GameMode.h"
#include "Horror28Character.h"
#include "UObject/ConstructorHelpers.h"

AHorror28GameMode::AHorror28GameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
