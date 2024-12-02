// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameSlot.generated.h"

class AUnitBase;
class UBoxComponent;

USTRUCT(Blueprintable)
struct FSGridPosition
{
	GENERATED_BODY()

	FSGridPosition() {}
	FSGridPosition(int col, int row) : Col(col), Row(row) {}
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 Col;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 Row;
};

UENUM(Blueprintable)
enum EGridState
{
	Default,
	Highlighted,
	Offensive,
	Supportive
};

UCLASS()
class AGameSlot : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGameSlot();

	UPROPERTY(BlueprintReadWrite)
	FSGridPosition GridPosition;

	UPROPERTY(EditDefaultsOnly)
	UBoxComponent* BoxComponent;

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* Plane;

	UPROPERTY(VisibleAnywhere)
	AUnitBase* Unit;
	
	EGridState GridState;

	void SpawnUnitHere(TSubclassOf<AUnitBase>& UnitClass);

	UFUNCTION()
	void SetState(EGridState NewState);

	UFUNCTION()
	void OnGridClicked(AActor* TouchedActor, FKey Key);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
