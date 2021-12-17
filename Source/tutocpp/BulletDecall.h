// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "BulletDecall.generated.h"


//class USphereComponent;
UCLASS()
class TUTOCPP_API ABulletDecall : public AActor
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    ABulletDecall();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    // Sphere collision component
    UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
        USphereComponent* CollisionComponent;
};
