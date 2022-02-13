// Copyright Epic Games, Inc. All Rights Reserved.

#include "tutocppCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "DrawDebugHelpers.h"

//////////////////////////////////////////////////////////////////////////
// AtutocppCharacter

AtutocppCharacter::AtutocppCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	JumpMaxCount = 2;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Mesh"));
	SceneComponent->SetRelativeLocation(FVector(80.f, 30.f, 0.f));
	SceneComponent->SetupAttachment(RootComponent);
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;
	
	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	WalkSpeed = 150;
	RunSpeed = 700;
	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

void AtutocppCharacter::RagdoLize()
{
	USkeletalMeshComponent* mesh = this->GetMesh();
	mesh->SetSimulatePhysics(true);
	mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	this->GetCharacterMovement()->DisableMovement();

	Controller = GetWorld()->GetFirstPlayerController();
	if(Controller==nullptr)
	{
		return;
	}
	Controller->UnPossess();

	GetWorldTimerManager().SetTimer(spawnDelay, this, &AtutocppCharacter::Respawn, 3, false);
}

void AtutocppCharacter::Respawn()
{
	
	AtutocppCharacter* Character = GetWorld()->SpawnActor<AtutocppCharacter>(ActorSpawned, FVector(-490.f, 503.f, 2862.f), FRotator(0.f,0.f,0.f));
	Controller = GetWorld()->GetFirstPlayerController();
	Controller->Possess(Character);
}

//////////////////////////////////////////////////////////////////////////
// Input

void AtutocppCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AtutocppCharacter::Fire);
	if(isGrabbed)
	{
		
	}
	else
	{
		PlayerInputComponent->BindAction("Grab", IE_Pressed, this, &AtutocppCharacter::Attach);
	}
	

	PlayerInputComponent->BindAxis("MoveForward", this, &AtutocppCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AtutocppCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AtutocppCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AtutocppCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &AtutocppCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &AtutocppCharacter::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &AtutocppCharacter::OnResetVR);
	PlayerInputComponent->BindAction("Run", IE_Pressed, this, & AtutocppCharacter::OnStartRun);
	PlayerInputComponent->BindAction("Run", IE_Released, this, &AtutocppCharacter::OnStopRun);
	//PlayerInputComponent->BindKey("E", IE_Pressed, this, &AtutocppCharacter::OnEPressed);
	//PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &AtutocppCharacter::OnStartCrouch);
	//PlayerInputComponent->BindAction("Crouch", IE_Released, this, &AtutocppCharacter::OnStopCrounch);
}


void AtutocppCharacter::BeginPlay()
{
	Super::BeginPlay();

	//GetCharacterMovement(WalkSpeed)
}

void AtutocppCharacter::Attach()
{
	
	FHitResult OutHit;
	FVector Start = FollowCamera->GetComponentLocation();
	FVector ForwardVector = FollowCamera->GetForwardVector();
	FVector End = ((ForwardVector * 1000.f) + Start);
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);
	DrawDebugLine(GetWorld(), Start, End, FColor::Green, true);

	bool isHit = GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility, CollisionParams);
	if(!isGrabbed)
	{
		
	
		if (isHit)
		{
			if (OutHit.bBlockingHit)
			{
				GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red,
					FString::Printf(TEXT("you are hitting: "), *OutHit.GetActor()->GetName()));
				GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red,
					FString::Printf(TEXT("Impact Point: "), *OutHit.ImpactPoint.ToString()));

				GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red,
					FString::Printf(TEXT("impact normal: "), *OutHit.ImpactNormal.ToString()));
				GrabbedActor = OutHit.GetActor();
				
				GrabbedActor->SetActorLocation(SceneComponent->GetComponentLocation());
				GrabbedActor->AttachToComponent(SceneComponent, FAttachmentTransformRules::KeepWorldTransform);
				OutHit.Component->SetSimulatePhysics(false);
				isGrabbed=true;
			}
		}
	}
	else
	{
		UPrimitiveComponent* object = Cast<UPrimitiveComponent>(GrabbedActor->GetComponentByClass(UPrimitiveComponent::StaticClass()));
		object->SetSimulatePhysics(true);
		GrabbedActor->DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
	}
}



void AtutocppCharacter::Fire()
{
	GLog->Log("PAn PAn PAn Bang Ban Bang");
	AProjectile* spawnedprojectile = GetWorld()->SpawnActor<AProjectile>(AProjectileA, SceneComponent->GetComponentLocation(), FollowCamera->GetComponentRotation());
	//AProjectile* bullet = GetWorld()->SpawnActor<AProjectile>(AProjectile, SceneComponent->GetComponentLocation(),SceneComponent->GetComponentRotation());

}
void AtutocppCharacter::OnStartRun()
{
	GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
}

void AtutocppCharacter::OnStopRun()
{
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}


void AtutocppCharacter::OnResetVR()
{
	// If tutocpp is added to a project via 'Add Feature' in the Unreal Editor the dependency on HeadMountedDisplay in tutocpp.Build.cs is not automatically propagated
	// and a linker error will result.
	// You will need to either:
	//		Add "HeadMountedDisplay" to [YourProject].Build.cs PublicDependencyModuleNames in order to build successfully (appropriate if supporting VR).
	// or:
	//		Comment or delete the call to ResetOrientationAndPosition below (appropriate if not supporting VR)
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AtutocppCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
		Jump();
}

void AtutocppCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
		StopJumping();
}

void AtutocppCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AtutocppCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AtutocppCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AtutocppCharacter::MoveRight(float Value)
{
	if ( (Controller != nullptr) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}
void AtutocppCharacter::HeathState(bool IsDamage,int Value)
{
	
	if(IsDamage)
	{
		health -= Value;
		GLog->Log("Et la il meurt");
	}
	else
	{
		health += Value;
		GLog->Log("Et la il soigne");
	}

	if (health <=0)
	{
		
		RagdoLize();
		isDead = true;
	}
}
