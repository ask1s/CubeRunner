// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCube.h"
#include "GameFramework/PlayerController.h"
#include "Components/CapsuleComponent.h"
// Sets default values
APlayerCube::APlayerCube()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APlayerCube::BeginPlay()
{
	Super::BeginPlay();


	//GetCapsuleComponent().OnComponentBeginOverlap.AddDynamic(this, &APlayerCube::OnOverlapBegin)
	
	CanMove = true;
}

// Called every frame
void APlayerCube::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CanMove)
	{
		const FVector ForwardPush = GetActorForwardVector() * ForwardSpeed * DeltaTime;
		AddActorWorldOffset(ForwardPush);
	}


	intTimer++;
	if (intTimer >= 60)
	{
		intTimer = 0;
		ForwardSpeed += 10;
	}
	GEngine->AddOnScreenDebugMessage(-1, 0.01f, FColor::Yellow, FString::Printf(TEXT("Speed: %.0f"), ForwardSpeed));

}

// Called to bind functionality to input
void APlayerCube::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCube::MoveRight);
}



void APlayerCube::MoveRight(float Value)
{
	if (CanMove)
	{
		const float Right = GetInputAxisValue("MoveRight") * MovementSpeed * GetWorld()->DeltaTimeSeconds;
		//FVector RightVector = FVector(0, Value * MovementSpeed * GetWorld()->DeltaTimeSeconds, 0);
		AddActorWorldOffset(FVector(0, Right, 0));
	}
}

void APlayerCube::RestartGame()
{
}
void APlayerCube::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}


