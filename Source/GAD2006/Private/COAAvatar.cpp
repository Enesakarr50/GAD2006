// Fill out your copyright notice in the Description page of Project Settings.


#include "COAAvatar.h"
#include "GameFramework/CharacterMovementComponent.h"

ACOAAvatar::ACOAAvatar() :
	RunSpeed(800),
	Stamina(100),
	MaxStamina(100),
	StaminaGainRate(8),
	StaminaDrainRate(5),
	StaminaRecoveryRate(5.0f),
	bStaminaDrained(false),
	bRunning(false)

{
	mSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	mSpringArm->TargetArmLength = 400.0f;
	mSpringArm->SetupAttachment(RootComponent);

	mCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("MainCamera"));
	mCamera->SetupAttachment(mSpringArm, USpringArmComponent::SocketName);
}

void ACOAAvatar::BeginPlay()
{
	Super::BeginPlay();
	mCamera->bUsePawnControlRotation = false;
	mSpringArm->bUsePawnControlRotation = true;
	bUseControllerRotationYaw = false;
}




// Called to bind functionality to input
void ACOAAvatar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("Turn", this, &ACharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &ACharacter::AddControllerPitchInput);

	PlayerInputComponent->BindAxis("MoveForward", this, &ACOAAvatar::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACOAAvatar::MoveRight);

	PlayerInputComponent->BindAction("Jump", EInputEvent::IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", EInputEvent::IE_Released, this, &ACOAAvatar::StopJumping);

	PlayerInputComponent->BindAction("Run", EInputEvent::IE_Pressed, this, &ACOAAvatar::RunPressed);
	PlayerInputComponent->BindAction("Run", EInputEvent::IE_Released, this, &ACOAAvatar::RunReleased);


}

void ACOAAvatar::RunPressed()
{
	// Koşma başlatma
	if (Stamina > 0.0f && !bStaminaDrained)
	{
		bRunning = true;
		UpdateMovement();
	}
}

void ACOAAvatar::RunReleased()
{
	// Koşmayı bırakma
	if (bRunning)
	{
		bRunning = false;
		UpdateMovement();
	}
}


void ACOAAvatar::MoveForward(float value)
{
	FRotator Rotation = GetController()->GetControlRotation();
	FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);
	FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	AddMovementInput(ForwardDirection, value);
	GEngine->AddOnScreenDebugMessage(0, 1, FColor::Cyan, FString::Printf(TEXT("Move Forward: %f"), value));
}

void ACOAAvatar::MoveRight(float value)
{
	FRotator Rotation = GetController()->GetControlRotation();
	FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);
	FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	AddMovementInput(RightDirection, value);

}
void ACOAAvatar::Tick(float DeltaTime)
{
	// Stamina kontrolü
	if (bRunning && !bStaminaDrained)
	{
		if (GetCharacterMovement()->MovementMode == MOVE_Walking)
		{
			Stamina -= StaminaDrainRate * DeltaTime;

			if (Stamina <= 0.0f)
			{
				bRunning = false;
				bStaminaDrained = true;
				Stamina = 0.0f;  // Stamina'nın negatif değerlere düşmesini engelle
				UpdateMovement();
			}
		}
	}
	else
	{
		if (Stamina < MaxStamina)
		{
			Stamina += StaminaRecoveryRate * DeltaTime;

			// Stamina maksimum seviyeye ulaşırsa normale döner
			if (Stamina >= MaxStamina)
			{
				bStaminaDrained = false;
				Stamina = MaxStamina;
			}
		}
	}
	

}
void ACOAAvatar::UpdateMovement()
{
	if (!bStaminaDrained)
	{
		if (bRunning)
		{
			GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
		}
		else
		{
			GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
		}
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	}

}








