// Fill out your copyright notice in the Description page of Project Settings.


#include "MyFirstActor.h"
#include "tutocppCharacter.h"
// Sets default values
AMyFirstActor::AMyFirstActor() 
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = MeshComponent;

	LetterCount = 10;

	MeshComponent->OnComponentHit.AddDynamic(this, &AMyFirstActor::OnHit);
}

// Called when the game starts or when spawned
void AMyFirstActor::BeginPlay()
{
	Super::BeginPlay();
	GLog->Log("BeginPlay" + FString::FromInt(LetterCount));
	
}

// Called every frame
void AMyFirstActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
void AMyFirstActor::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	AtutocppCharacter* Character = Cast<AtutocppCharacter>(OtherActor);
	if (Character == nullptr)
	{
		return;
	}
	Character->LaunchCharacter(FVector(0, 0, 1000), false, true);
	GLog->Log("Onhit");
}

