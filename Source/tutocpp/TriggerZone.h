// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "tutocppCharacter.h"
#include "TriggerZone.generated.h"

UCLASS()
class TUTOCPP_API ATriggerZone : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATriggerZone();
	UPROPERTY()
		class USceneComponent* SceneComponent;
	UPROPERTY()
		FTimerHandle timer;
	UPROPERTY(VisibleAnywhere)
		class UStaticMeshComponent* MeshComponent;
	UFUNCTION()
		void OnOverlapBegin(class AActor* OverlappedActor, class AActor* OtherActor);
	UFUNCTION()
		void OnOverlapEnd(class AActor* OverlappedActor, class AActor* OtherActor);
	UPROPERTY()
		FTimerHandle UnusedHandle;
	UPROPERTY(EditAnywhere)
		bool isDamage;
	UFUNCTION()
		void IsDamageZone(AtutocppCharacter* Character);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
};
