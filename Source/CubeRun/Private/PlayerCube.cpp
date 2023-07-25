// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCube.h"

#include "GameFramework/PlayerController.h"

#include "Components/StaticMeshComponent.h"

#include "ObstaclesLogic.h"
#include "RightTrampoline.h"
#include "LeftTrampoline.h"
#include "Booster_Protection.h"
#include "Booster_DoubleJump.h"

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
	MeshComponent->OnComponentHit.AddDynamic(this, &APlayerCube::OnHit);
	
	CanMove = true;
	isProtected = false;
	JumpCount = 1;
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
		
		if (DoubleJump_TimerStarted)
			DoubleJump_Timer++;
	}
	if (DoubleJump_Timer >= 10)
	{
		JumpCount = 1;
		DoubleJump_TimerStarted = false;
		DoubleJump_Timer = 0;
	}




	if (GetActorLocation().Z <= -30)
	{
		FTimerHandle UnusedHandle;
		GetWorldTimerManager().SetTimer(UnusedHandle, this, &APlayerCube::RestartGame, 1.f, false);
	}

	GEngine->AddOnScreenDebugMessage(-1, 0.01f, FColor::Red, FString::Printf(TEXT("Speed: %.0f"), ForwardSpeed));
	GEngine->AddOnScreenDebugMessage(-2, 0.01f, FColor::Red, FString::Printf(TEXT("TimesJumped: %.0f"), TimesJumped));
}

// Called to bind functionality to input
void APlayerCube::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCube::MoveRight);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &APlayerCube::Jump);
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
void APlayerCube::Jump()
{
	if ((JumpCount > TimesJumped) && CanMove)
	{
		FVector JumpVector = FVector(0, 0, JumpForce);
		MeshComponent->AddImpulse(JumpVector, "", true);

		TimesJumped++;
	}
}
void APlayerCube::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != nullptr)
	{
		AObstaclesLogic* Obstacle = Cast<AObstaclesLogic>(OtherActor);
		ARightTrampoline* rTrampoline = Cast<ARightTrampoline>(OtherActor);
		ALeftTrampoline* lTrampoline = Cast<ALeftTrampoline>(OtherActor);
		ABooster_Protection* BoosterProtection = Cast<ABooster_Protection>(OtherActor);
		ABooster_DoubleJump* BoosterDoubleJump = Cast<ABooster_DoubleJump>(OtherActor);

		if (Obstacle)
		{
			if (!isProtected)
			{
				MeshComponent->Deactivate();
				MeshComponent->SetVisibility(false);

				CanMove = false;

				FTimerHandle UnusedHandle;
				GetWorldTimerManager().SetTimer(UnusedHandle, this, &APlayerCube::RestartGame, 2.f, false);
			}
			else
			{
				isProtected = false;
				Obstacle->Destroy();
			}
			

		}
		if (rTrampoline || lTrampoline)
		{
			FVector tempVector = FVector(0.f, 0.f, 0.f);
			if (rTrampoline)
			{
				tempVector = FVector(0.f, -trampolineBackImpusle, trampolineUpImpusle);
				rTrampoline->Destroy();
			}
			else if (lTrampoline)
			{
				tempVector = FVector(0.f, trampolineBackImpusle, trampolineUpImpusle);
				lTrampoline->Destroy();

			}
			MeshComponent->AddImpulse(tempVector, "", true);
			
		}
		if (BoosterProtection)
		{
			isProtected = true;
			BoosterProtection->Destroy();
		}
		if (BoosterDoubleJump)
		{
			JumpCount = 2;
			DoubleJump_TimerStarted = true;
			DoubleJump_Timer = 0;
			BoosterDoubleJump->Destroy();

		}

	}
}

void APlayerCube::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	float HitDirection = Hit.Normal.Z;

	if (HitDirection >= 0)
	{
		TimesJumped = 0;
	}
}


