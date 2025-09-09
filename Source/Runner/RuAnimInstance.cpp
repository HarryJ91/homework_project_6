// Fill out your copyright notice in the Description page of Project Settings.


#include "RuAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

URuAnimInstance::URuAnimInstance()
{

}

void URuAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Owner = Cast<ACharacter>(TryGetPawnOwner());

	if (Owner)
	{
		Movement = Owner->GetCharacterMovement();
	}
}

void URuAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (Movement)
	{
		Velocity = Movement->Velocity;
		MoveSpeed = Velocity.Size2D();
		isFalling = Movement->IsFalling();
		bIsPickingUp = false;
		 = nullptr;
	}

	// If we were flagged picking up, check if the montage finished and clear the flag



	
