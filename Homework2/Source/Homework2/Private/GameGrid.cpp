// Fill out your copyright notice in the Description page of Project Settings.


#include "GameGrid.h"
#include "GameSlot.h"
#include "ShaderParameterMacros.h"
#include "Components/BoxComponent.h"

AGameGrid* AGameGrid::GameGrid = nullptr;

// Sets default values
AGameGrid::AGameGrid()
	
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RowsNum = 8;
	ColsNum = 8;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	GameGrid = this;
}

void AGameGrid::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if(!GameSlotClass) return;

	for (auto Grid : GridActors)
	{
		if(IsValid(Grid))
		{
			Grid->DestroyComponent();
		}
		
	}

	GridActors.Empty();

	AGameSlot* Slot = GameSlotClass->GetDefaultObject<AGameSlot>();

	if (!Slot) return;

	FVector Extends = Slot->BoxComponent->GetScaledBoxExtent() * 2;

	for(int i = 0; i < RowsNum; i++)
	{
		for(int j = 0; j < ColsNum; j++)
		{
			FName GridName(FString::Printf(TEXT("Slot%dx%d"), j, i));
			auto Grid = NewObject<UChildActorComponent>(this, UChildActorComponent::StaticClass(), GridName);
			Grid->RegisterComponent();
			Grid->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
			Grid->SetRelativeLocation(
				FVector(
					(RowsNum - i - 1) * Extends.X - (RowsNum * 0.5f - 0.5f) * Extends.X,
					j * Extends.Y - (ColsNum * 0.5f - 0.5f) * Extends.Y, 0));

			GridActors.Add(Grid);

			Grid->SetChildActorClass(GameSlotClass);
			AGameSlot* GameSlot = Cast<AGameSlot>(Grid->GetChildActor());

			GameSlot->SetActorLabel(GridName.ToString());
			GameSlot->GridPosition.Col = j;
			GameSlot->GridPosition.Row = i;
		}
	}
}

// Called when the game starts or when spawned
void AGameGrid::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGameGrid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

AGameSlot* AGameGrid::GetSlot(FSGridPosition& Position)
{
	int GridIndex = Position.Row * ColsNum + Position.Col;
	if(GridActors.IsValidIndex(GridIndex))
	{
		return Cast<AGameSlot>(GridActors[GridIndex]->GetChildActor());
	}
	return nullptr;
}

AGameSlot* AGameGrid::FindSlot(FSGridPosition& Position)
{
	if(GameGrid)
	{
		return GameGrid->GetSlot(Position);
	}

	return nullptr;
}



