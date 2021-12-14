// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Lava.generated.h"

UCLASS()
class TUTOCPP_API ALava : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALava();
	UPROPERTY(VisibleAnywhere)
		class UStaticMeshComponent* MeshComponent;
	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor, UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
