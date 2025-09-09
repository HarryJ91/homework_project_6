// Fill out your copyright notice in the Description page of Project Settings.

//#include "RAnimInstance.h"
#include "Werewolf.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/SkeletalMesh.h"
#include "Components/PoseableMeshComponent.h"
#include "Math/Transform.h"
#include "Components/InputComponent.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "RuAnimInstance.h"


/*DEFINE_LOG_CATEGORY(LogTemplateCharacter);  <- this is not needed */

// Sets default values
AWerewolf::AWerewolf()
{
	AutoPossessPlayer = EAutoReceiveInput::Player0;
	/*
	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	CapsuleComponent->SetupAttachment(RootComponent);
	CapsuleComponent->SetCapsuleSize(42.f, 96.f);
	*/

	//create the static mesh component
	//MySkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MySkeletalMeshComponent"));
	//MySkeletalMeshComponent->SetupAttachment(RootComponent);
	GetMesh()->SetupAttachment(RootComponent);
	//RootComponent = MySkeletalMeshComponent; //set as root component
		
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SkeletalMeshAsset(TEXT("/Game/Characters/Heroes/Mannequin/Meshes/SKM_Manny.SKM_Manny")); // Replace with your mesh path")
	if (SkeletalMeshAsset.Succeeded())
	{
		GetMesh()->SetSkeletalMeshAsset(SkeletalMeshAsset.Object);
	}
	
	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimiInstance
	(TEXT("/Game/Assets/Animation_BlendSpace/AnimInstanceBP.AnimInstanceBP_C"));
	if (AnimiInstance.Class)
	{
		GetMesh()->SetAnimInstanceClass(AnimiInstance.Class);
	}

	/*static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstance
	(TEXT("/Game/PickFruit_UE_Anim1.PickFruit_UE_Anim1"));
	if (AnimInstance.Class)
	{
		GetMesh()->SetAnimInstanceClass(AnimInstance.Class);
	}*/

	

	FRotator NewRotation = FRotator(0.0f, 270.f, 0.0f);
	GetMesh()->SetWorldRotation(NewRotation);
	FVector NewLocation1 = FVector(0.0f, 0.0f, -90.0f);
	GetMesh()->SetWorldLocation(NewLocation1);




	// Set size for collision capsule
	//GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = false; // character moves in the direction of input
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // at this rotation rate

	//this can be tweaked in the Character BluePrint
	GetCharacterMovement()->JumpZVelocity = 450.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 150.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	

 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create a camera boom (pulls in twoards the player if there is a collision)

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 130.0f; // Camera Distance
	CameraBoom->bUsePawnControlRotation = true; // rotate the arm based on the controller
	FVector NewLocation = FVector(0.0f, 65.0f, 55.0f);
	CameraBoom->SetRelativeLocation(NewLocation);
	FRotator NewRotation1 = FRotator(0.0f, 0.0f, 0.0f); // There's a function that can change both location and rotation at the same time.
	CameraBoom->SetRelativeRotation(NewRotation1); //make sure to change the spring arm rotation.

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // attaching the camera to the boom
	FollowCamera->bUsePawnControlRotation = false; //Camera doesn't rotate relative to arm

}

// Called when the game starts or when spawned
void AWerewolf::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PlayerController = Cast< APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem< UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(WerewolfContext, 0);
		}
	
	}
	if (FollowCamera)
	{
		FollowCamera->SetFieldOfView(DefaultFOV);
	}
	
	
	

	

	

	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			if (WerewolfContext)
				Subsystem->AddMappingContext(WerewolfContext, 0);
		}
	}

}

 //need to understand this even little bit in order to use it
/*void AWerewolf::Pickup(const FInputActionValue& Value)
{
	FVector Start = FollowCamera->GetComponentLocation();
	FVector ForwardVector = FollowCamera->GetForwardVector();
	FVector End = Start + (ForwardVector * 500.0f); // 500 units in front

	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, Params))
	{
		AActor* HitActor = HitResult.GetActor();
		if (HitActor)
		{
			UE_LOG(LogTemp, Warning, TEXT("Picked up: %s"), *HitActor->GetName());

			//just destroy what you hit
			//HitActor->Destroy();
		}
	}
}*/



/*void AWerewolf::Pickup(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Warning, TEXT("Pickup action triggered"));

	if (USkeletalMeshComponent* MeshComp = GetMesh())
	{
		if (UAnimInstance* AnimInstance = MeshComp->GetAnimInstance())
		{
			if (URuAnimInstance* RuAnim = Cast<URuAnimInstance>(AnimInstance))
			{
				// Tell the anim instance to play (it uses its internal array to pick one)
				RuAnim->PlayRandomPickupMontage();
			}
		}
	}
}
*/

void AWerewolf::Pickup(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Warning, TEXT("Pickup action triggered"));

	if (USkeletalMeshComponent* MeshComp = GetMesh())
	{
		if (URuAnimInstance* RuAnim = Cast<URuAnimInstance>(MeshComp->GetAnimInstance()))
		{
			// Use current actor rotation (facing) and WalkSpeed threshold from your character
			RuAnim->PlayPickupBasedOnMovement(GetVelocity(), GetActorRotation(), WalkSpeed);
		}
	}
}


void AWerewolf::Move(const FInputActionValue& Value)
{
	const FVector2D MovementVector = Value.Get<FVector2D>();
	
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	AddMovementInput(ForwardDirection, MovementVector.X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	AddMovementInput(RightDirection, MovementVector.Y);

	UE_LOG(LogTemp, Warning, TEXT("Move Input: X=%f Y=%f"), MovementVector.X, MovementVector.Y);

}
void AWerewolf::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxisVector = Value.Get<FVector2D>();

	AddControllerPitchInput(LookAxisVector.Y * LookSensitivityPitch);
	AddControllerYawInput(LookAxisVector.X * LookSensitivityPitch);
}


// Called every frame
void AWerewolf::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (FollowCamera)
	{
		float TargetFOV = bIsAiming ? AimFOV : DefaultFOV;
		float NewFOV = FMath::FInterpTo(FollowCamera->FieldOfView, TargetFOV, DeltaTime, ZoomInterSpeed);
		FollowCamera->SetFieldOfView(NewFOV);
	}
}

// Called to bind functionality to input
void AWerewolf::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (!WerewolfContext)
		WerewolfContext = LoadObject<UInputMappingContext>(nullptr, TEXT("/Script/EnhancedInput.InputMappingContext'/Game/Assets/Input/IMC_Werewolf.IMC_Werewolf'"));

	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(WerewolfContext, 0);
		}
	}

	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInput->BindAction(PickupAction, ETriggerEvent::Started, this, &AWerewolf::Pickup);
	}
	// this is the way to add specific c++ inputcomponent
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (!MovementAction)
			MovementAction = LoadObject<UInputAction>(nullptr, TEXT("/Script/EnhancedInput.InputAction'/Game/Assets/Input/IA_WerewolfMove.IA_WerewolfMove'"));

		if (!LookAction)
			LookAction = LoadObject<UInputAction>(nullptr, TEXT("/Script/EnhancedInput.InputAction'/Game/Assets/Input/IA_WerewolfLook.IA_WerewolfLook'"));

		if (!JumpAction)
			JumpAction = LoadObject<UInputAction>(nullptr, TEXT("/Script/EnhancedInput.InputAction'/Game/Assets/Input/IA_WerewolfJump.IA_WerewolfJump'"));

		if (!SprintAction)
			SprintAction = LoadObject<UInputAction>(nullptr, TEXT("/Script/EnhancedInput.InputAction'/Game/Assets/Input/IA_WerewolfSprint.IA_WerewolfSprint'"));

		if (!AimAction)
			AimAction = LoadObject<UInputAction>(nullptr, TEXT("/Script/EnhancedInput.InputAction'/Game/Assets/Input/IA_WerewolfAim.IA_WerewolfAim'"));
		
		if (!PickupAction)
			PickupAction = LoadObject<UInputAction>(nullptr, TEXT("/Script/EnhancedInput.InputAction'/Game/Assets/Input/IA_Pickup.IA_Pickup'"));

		EnhancedInputComponent->BindAction(MovementAction, ETriggerEvent::Triggered, this, &AWerewolf::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AWerewolf::Look);

		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &AWerewolf::SprintStart);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &AWerewolf::SprintEnd);

		EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Started, this, &AWerewolf::AimStart);
		EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Completed, this, &AWerewolf::AimEnd);

		
		EnhancedInputComponent->BindAction(PickupAction, ETriggerEvent::Started, this, &AWerewolf::Pickup);
		
	}

}



void AWerewolf::SprintStart()
{
	GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
}

void AWerewolf::SprintEnd()
{
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

void AWerewolf::AimStart(const FInputActionValue& Value)
{
	bIsAiming = true;
}

void AWerewolf::AimEnd(const FInputActionValue& Value)
{
	bIsAiming = false;
}
