// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "InputActionValue.h"
#include "Werewolf.generated.h"


class UPoseableMeshComponent;
class UCapsuleComponent;
class USkeletalMeshComponent;
class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

UCLASS()
class RUNNER_API AWerewolf : public ACharacter
{
	GENERATED_BODY()

private:


	
	/* Jump Input Action */
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, meta = (AllowPrivateAccess = "true"))
	//UInputAction* JumpAction;


	

public:
	// Sets default values for this character's properties
	AWerewolf();
	/*
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Animation")
	float Speed;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Animation")
	bool bIsJumping;

	virtual void (float DeltaSeconds) override;
	*/
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	//UPROPERTY(EditAnywhere, Category = "Input")
	//UInputMappingContext* WerewolfContext;

	//UPROPERTY(EditAnywhere, Category = "Input")
	//UInputAction* MovementAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float WalkSpeed = 150.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float SprintSpeed = 300.f;

	//UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "Input")
	//UInputAction* SprintAction;

	/* Look Input Action */
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, meta = (AllowPrivateAccess = "true"))
	//UInputAction* LookAction;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
	float LookSensitivityYaw = 0.3f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
	float LookSensitivityPitch = 0.3f;

	void SprintStart();
	void SprintEnd();

	/* Called for movement input */
	void Move(const FInputActionValue& Value);

	/* Called For looking input */
	void Look(const FInputActionValue& Value);

	//virtual void NotifyControllerChanged() override;

		// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	

	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	//UPoseableMeshComponent* PoseableMesh;

	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	//UCapsuleComponent* CapsuleComponent;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	//UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = Input, meta = (AloowPrivateAccess = "true"))
	//UInputAction* AimAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|Zoom")
	float DefaultFOV = 90.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|Zoom")
	float AimFOV = 60.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|Zoom")
	float ZoomInterSpeed = 15.0f;

	bool bIsAiming = false;

	void AimStart(const FInputActionValue& Value);
	void AimEnd(const FInputActionValue& Value);

	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SkeletalMesh")
	USkeletalMeshComponent* MySkeletalMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Character", meta = (AllowPrivateAcess  = "true"))
	USkeletalMeshComponent* HoodMesh;

	/* Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/* Follow Camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
	//class UInputAction* PickupAction;
	UFUNCTION()
	void Pickup(const FInputActionValue& Value);

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category = "Animation")
	bool bIsPickingUp = false;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	//TArray<UAnimMontage*> PickupMontages;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Animation")
	class UAnimMontage* SpecificPickupMontage;

	// Returns CameraBoom Subject
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	// Returns Follow Camera subobject
	FORCEINLINE class UCameraComponent* GetFolllowCamera() const { return FollowCamera; }

	protected:
		UPROPERTY()
		UInputMappingContext* WerewolfContext = nullptr;

		UPROPERTY()
		UInputAction* MovementAction = nullptr;

		UPROPERTY()
		UInputAction* LookAction = nullptr;

		UPROPERTY()
		UInputAction* JumpAction = nullptr;

		UPROPERTY()
		UInputAction* SprintAction = nullptr;

		UPROPERTY()
		UInputAction* AimAction = nullptr;
		
		UPROPERTY()
		UInputAction* PickupAction = nullptr;
		
};
