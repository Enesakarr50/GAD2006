// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameSlot.h"
#include "UnitBase.h"
#include "Commands/Command.h"
#include "GameManager.generated.h"


class AGameGrid;



USTRUCT(BlueprintType)
struct FUnitInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TSubclassOf<AUnitBase> UnitClass;

	UPROPERTY(EditAnywhere)
	FSGridPosition StartPosition;	
};

USTRUCT(BlueprintType)
struct FSLevelInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TArray<FUnitInfo> Units;
};

UCLASS()
class HOMEWORK2_API AGameManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGameManager();

	void OnActorClicked(AActor* Actor, FKey Button);

	void CreateLevelActors(FSLevelInfo& LevelInfo);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int CurrentLevel;

	UPROPERTY(EditAnywhere)
	TArray<FSLevelInfo> Levels;

	UPROPERTY(EditAnywhere)
	AGameGrid* GameGrid;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	bool UndoLastMove();

	private:
	AUnitBase* PlayerUnit;

	TArray<TSharedRef<Command>> CommandPool;
	TSharedPtr<Command> CurrentCommand;

};
