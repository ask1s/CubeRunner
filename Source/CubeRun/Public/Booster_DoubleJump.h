// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Booster_DoubleJump.generated.h"

UCLASS()
class CUBERUN_API ABooster_DoubleJump : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABooster_DoubleJump();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
