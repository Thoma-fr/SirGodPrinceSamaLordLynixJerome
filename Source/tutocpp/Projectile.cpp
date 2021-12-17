
#include "Engine/DecalActor.h"
#include "Components/DecalComponent.h"
#include "Projectile.h"

// Sets default values
AProjectile::AProjectile()
{

	PrimaryActorTick.bCanEverTick = true;

    if (!RootComponent)
    {
        CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
        
        CollisionComponent->InitSphereRadius(15.0f);

        RootComponent = CollisionComponent;
    }
    if (!ProjectileMovementComponent)
    {
        // Use this component to drive this projectile's movement.
        ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
        ProjectileMovementComponent->SetUpdatedComponent(CollisionComponent);
        ProjectileMovementComponent->InitialSpeed = 3000.0f;
        ProjectileMovementComponent->MaxSpeed = 3000.0f;
        ProjectileMovementComponent->bRotationFollowsVelocity = true;
        ProjectileMovementComponent->bShouldBounce = true;
        ProjectileMovementComponent->Bounciness = 0.3f;
        ProjectileMovementComponent->ProjectileGravityScale = 0.0f;
    }
    //OnActorBeginOverlap->OnComponentHit.AddDynamic(this, &AProjectile::OnOverlapBegin);
    InitialLifeSpan = 5;
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
    OnActorBeginOverlap.AddDynamic(this, &AProjectile::OnOverlapBegin);
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}







void AProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent,
	FVector NormalImpulse, const FHitResult& Hit)
{
	GLog->Log("bullet go boing boing bing bong bong ");
}


void AProjectile::FireInDirection(const FVector& ShootDirection)
{
    ProjectileMovementComponent->Velocity = ShootDirection * ProjectileMovementComponent->InitialSpeed;
}

void AProjectile::OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
{
    GLog->Log("bullet go boing boing bing bong bong ");
    FRotator decallRot = GetActorRotation() + FRotator(-90, 0, 0);

    ADecalActor* decal = GetWorld()->SpawnActor<ADecalActor>(GetActorLocation(), decallRot);
    decal->SetDecalMaterial(DecallMat);
    decal->GetDecal()->DecalSize - FVector(100, 100, 100);
    decal->GetDecal()->SetFadeOut(1, 3, true);
}

