// Fill out your copyright notice in the Description page of Project Settings.


#include "Lava.h"
#include "tutocppCharacter.h"
// Sets default values
ALava::ALava()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LAVA"));
	RootComponent = MeshComponent;

	MeshComponent->OnComponentBeginOverlap.AddDynamic(this, &ALava::OnOverlapBegin);
}

// Called when the game starts or when spawned
void ALava::BeginPlay()
{
	Super::BeginPlay();
	
	
}

// Called every frame
void ALava::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
void ALava::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AtutocppCharacter* Character = Cast<AtutocppCharacter>(OtherActor);
	if (Character == nullptr)
	{
		return;
	}
	GLog->Log("Begin Overlap----------");
	Character->HeathState(true, 100);

}


