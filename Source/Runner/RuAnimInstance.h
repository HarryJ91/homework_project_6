// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "RuAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class RUNNER_API URuAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	URuAnimInstance();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montages")
	UAnimMontage* IdlePickupMontages;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montages")
	TArray<UAnimMontage*> WalkPickupMontages;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montages")
	TArray<UAnimMontage*> JogPickupMontages;

	// Call from character to play one pickup montage (anim instance chooses random)
	UFUNCTION(BlueprintCallable, Category = "Montages")
	void PlayRandomPickupMontage();

	// Flag you already use (optional)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
	bool bIsPickingUp = false;

	// Keep track of which montage is playing so we can clear the flag when finished
	
	
	
protected:
	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Character")
	class ACharacter* Owner;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Character")
	class UCharacterMovementComponent* Movement;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	FVector Velocity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	float MoveSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	bool isFalling = false;

};

