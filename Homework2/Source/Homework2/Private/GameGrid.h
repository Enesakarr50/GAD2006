// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameGrid.generated.h"

struct FSGridPosition;
class AGameSlot;

UCLASS()
class AGameGrid : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGameGrid();

	virtual void OnConstruction(const FTransform& Transform) override;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AGameSlot> GameSlotClass;

	UPROPERTY(EditAnywhere)
	int RowsNum;

	UPROPERTY(EditAnywhere)
	int ColsNum;

	UPROPERTY(VisibleAnywhere)
	TArray<UChildActorComponent*> GridActors;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	AGameSlot* GetSlot(FSGridPosition& Position);
	static AGameSlot* FindSlot(FSGridPosition& Position);

private:
	static AGameGrid* GameGrid;

};
