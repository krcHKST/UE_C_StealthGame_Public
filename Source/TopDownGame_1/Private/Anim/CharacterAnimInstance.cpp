// Fill out your copyright notice in the Description page of Project Settings.


#include "Anim/CharacterAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"

void UCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
    Super::NativeUpdateAnimation(DeltaSeconds);
    ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwningActor());
    if (OwnerCharacter != nullptr)
    {
        //‘¬“x
        WalkSpeed = OwnerCharacter->GetVelocity().Size();

        //‹ó’†‚É‚¢‚é‚©‚Ç‚¤‚©‚ðŽæ“¾‚·‚é
        IsFall = OwnerCharacter->GetCharacterMovement()->IsFalling();
    }
}
