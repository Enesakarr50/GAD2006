// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TBPlayerController.generated.h"

class AGameManager;
/**
 * 
 */
UCLASS()
class HOMEWORK2_API ATBPlayerController : public APlayerController
{
	GENERATED_BODY()


	public:
	void BeginPlay() override;
	
	void OnActorClicked(AActor* Slot, FKey Key);
	
	UPROPERTY(BLueprintReadWrite)
	AGameManager* GameManager;
	
};
