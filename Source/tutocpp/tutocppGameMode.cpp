// Copyright Epic Games, Inc. All Rights Reserved.

#include "tutocppGameMode.h"
#include "tutocppCharacter.h"
#include "UObject/ConstructorHelpers.h"

AtutocppGameMode::AtutocppGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
