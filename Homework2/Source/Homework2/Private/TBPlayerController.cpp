// Fill out your copyright notice in the Description page of Project Settings.


#include "TBPlayerController.h"
#include "GameManager.h"


void ATBPlayerController::BeginPlay()
{
	Super::BeginPlay();
	bEnableClickEvents = true;
	bShowMouseCursor = true;
	ClickEventKeys.AddUnique(EKeys::RightMouseButton);


}

void ATBPlayerController::OnActorClicked(AActor* Actor, FKey Key)
{
	UE_LOG(LogTemp, Display, TEXT("OnActorClicked %s - %s"), *Actor->GetActorLabel(), *Key.ToString());
	if(GameManager) GameManager->OnActorClicked(Actor, Key);
}
