// Fill out your copyright notice in the Description page of Project Settings.


#include "GameSlot.h"

#include "TBPlayerController.h"
#include "Components/BoxComponent.h"

#include "PhysicsEngine/ShapeElem.h"
#include "UnitBase.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"


// Sets default values
AGameSlot::AGameSlot()
{
	
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	BoxComponent = CreateDefaultSubobject<UBoxComponent>("Box");
	BoxComponent->SetupAttachment(RootComponent);
	BoxComponent->SetBoxExtent(FVector(50,50, 2));
	BoxComponent->SetCollisionResponseToAllChannels(ECR_Block);

	Plane = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Plane"));
	Plane->SetupAttachment(RootComponent);
	
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SetState(Default);

}

void AGameSlot::SpawnUnitHere(TSubclassOf<AUnitBase>& UnitClass)
{
	FVector Location = GetActorLocation();
	FRotator Rotation = GetActorRotation();
	AUnitBase* NewUnit = GetWorld()->SpawnActor<AUnitBase>(UnitClass, Location, Rotation);
	UE_LOG(LogTemp, Warning, TEXT("Attempting to spawn unit at Location: %s, Rotation: %s"), *Location.ToString(), *Rotation.ToString());
	if (NewUnit) NewUnit->AssignToSlot(this);
}

void AGameSlot::SetState(EGridState NewState)
{
	GridState = NewState;

	static ConstructorHelpers::FObjectFinder<UMaterialInterface>
			DefaultSlotMaterial(TEXT("/Script/Engine.Material'/Game/Materials/MAT_GridSlot.MAT_GridSlot'"));
	static ConstructorHelpers::FObjectFinder<UMaterialInterface>
			HighlightSlotMaterial(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Materials/MAT_GridSlot_Highlighted.MAT_GridSlot_Highlighted'"));
	static ConstructorHelpers::FObjectFinder<UMaterialInterface>
			OffensiveSlotMaterial(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Materials/MAT_GridSlot_Offensive.MAT_GridSlot_Offensive'"));
	static ConstructorHelpers::FObjectFinder<UMaterialInterface>
			SupportiveSlotMaterial(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Materials/MAT_GridSlot_Supportive.MAT_GridSlot_Supportive'"));

	switch (NewState)
	{
	case Default:
			Plane->SetMaterial(0, DefaultSlotMaterial.Object);
			break;
	case Highlighted:
			Plane->SetMaterial(0, HighlightSlotMaterial.Object);
			break;
	case Offensive:
			Plane->SetMaterial(0, OffensiveSlotMaterial.Object);
			break;
	case Supportive:
			Plane->SetMaterial(0, SupportiveSlotMaterial.Object);
			break;
	}
	
}

void AGameSlot::OnGridClicked(AActor* TouchedActor, FKey Key)
{
	if (ATBPlayerController* PlayerController = GetWorld()->GetFirstPlayerController<ATBPlayerController>())
	{
		PlayerController->OnActorClicked(this, Key);
		UE_LOG(LogTemp, Warning, TEXT("OnGridClicked"));
	}
	
}

// Called when the game starts onur when spawned
void AGameSlot::BeginPlay()
{
	Super::BeginPlay();
	OnClicked.AddDynamic(this, &AGameSlot::OnGridClicked);
	
}

// Called every frame
void AGameSlot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

