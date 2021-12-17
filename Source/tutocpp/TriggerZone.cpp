// Fill out your copyright notice in the Description page of Project Settings.


#include "TriggerZone.h"
#include "tutocppCharacter.h"
#include "Engine/TriggerVolume.h"
#include "DrawDebugHelpers.h"
// Sets default values
ATriggerZone::ATriggerZone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LAVA"));
	RootComponent = MeshComponent;
	//isDamage;
	
	
}

void ATriggerZone::OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
{
	GLog->Log("TriggerZone arh");

	AtutocppCharacter* Character = Cast<AtutocppCharacter>(OtherActor);
	FTimerDelegate timerDelegate;
	timerDelegate.BindUFunction(this, FName("IsDamageZone"), Character);
	GetWorldTimerManager().SetTimer(UnusedHandle, timerDelegate, 1, true);
	if (Character == nullptr)
	{
		return;
	}
	
	
}
void ATriggerZone::IsDamageZone(AtutocppCharacter* Character)
{
	Character->HeathState(isDamage, value);
}
void ATriggerZone::OnOverlapEnd(AActor* OverlappedActor, AActor* OtherActor)
{
	GLog->Log("TriggerZone out");
	AtutocppCharacter* Character = Cast<AtutocppCharacter>(OtherActor);
	GetWorldTimerManager().ClearTimer(UnusedHandle);
	if (Character == nullptr)
	{
		return;
	}
	
}



// Called when the game starts or when spawned
void ATriggerZone::BeginPlay()
{
	Super::BeginPlay();
	OnActorBeginOverlap.AddDynamic(this, &ATriggerZone::OnOverlapBegin);
	OnActorEndOverlap.AddDynamic(this, &ATriggerZone::OnOverlapEnd);
	
}

// Called every frame
void ATriggerZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

