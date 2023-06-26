// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LevelGenerator.generated.h"

class ABaseLevel;

UCLASS()
class CUBERUN_API ALevelGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALevelGenerator();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	APawn* Player;

	UPROPERTY(EditAnywhere)
		TSubclassOf<ABaseLevel> Level1;

	UPROPERTY(EditAnywhere)
		TSubclassOf<ABaseLevel> Level2;

	TArray<ABaseLevel*> LevelList;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void SpawnLevel(bool isStart);

	int RandomLevel;
	FVector SpawnLocation = FVector();
	FRotator SpawnRotation = FRotator();
	FActorSpawnParameters SpawnInfo = FActorSpawnParameters();

	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};