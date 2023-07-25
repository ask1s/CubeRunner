// Fill out your copyright notice in the Description page of Project Settings.


#include "Booster_Protection.h"

// Sets default values
ABooster_Protection::ABooster_Protection()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABooster_Protection::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABooster_Protection::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

