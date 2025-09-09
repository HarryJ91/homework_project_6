// Fill out your copyright notice in the Description page of Project Settings.

#include "MyGameMode.h"
#include "Werewolf.h"
#include "UObject/ConstructorHelpers.h"
#include "GameFramework/Actor.h"

AMyGameMode::AMyGameMode()
{
	DefaultPawnClass = AWerewolf::StaticClass();
}

void AMyGameMode::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("GameMode has Started"))
}