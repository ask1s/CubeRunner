// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCube.h"

#include "GameFramework/PlayerController.h"

#include "Components/StaticMeshComponent.h"

#include "ObstaclesLogic.h"
#include "RightTrampoline.h"
#include "LeftTrampoline.h"

#include "Engine.h"

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

	MeshComponent = Cast<UStaticMeshComponent>(GetComponentByClass(UStaticMeshComponent::StaticClass()));
	MeshComponent->OnComponentBeginOverlap.AddDynamic(this, &APlayerCube::OnOverlapBegin);

	//Attempt #1
	//MeshComponent->BodyInstance.bLockXRotation = true;
	//MeshComponent->BodyInstance.bLockYRotation = true;
	//MeshComponent->BodyInstance.bLockZRotation = true;
	
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

	//Attempt #2
	//CurrentRotation = GetActorRotation();
	//SetActorRotation(FRotator(0.f, CurrentRotation.Yaw, 0.f));

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
	UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()));
}
void APlayerCube::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != nullptr)
	{
		AObstaclesLogic* Obstacle = Cast<AObstaclesLogic>(OtherActor);
		ARightTrampoline* rTrampoline = Cast<ARightTrampoline>(OtherActor);
		ALeftTrampoline* lTrampoline = Cast<ALeftTrampoline>(OtherActor);

		if (Obstacle)
		{
			MeshComponent->Deactivate();
			MeshComponent->SetVisibility(false);

			CanMove = false;

			FTimerHandle UnusedHandle;
			GetWorldTimerManager().SetTimer(UnusedHandle, this, &APlayerCube::RestartGame, 2.f, false);

		}
		if (rTrampoline || lTrampoline)
		{
			FVector tempVector = FVector(0.f, 0.f, 0.f);
			if (rTrampoline)
			{
				tempVector = FVector(0.f, -50.f, 200.f);
			}
			else if (lTrampoline)
			{
				tempVector = FVector(0.f, 50.f, 200.f);
			}
			MeshComponent->AddImpulse(tempVector, "", true);
		}

	}
}


