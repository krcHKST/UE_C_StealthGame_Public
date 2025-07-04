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
        //速度
        WalkSpeed = OwnerCharacter->GetVelocity().Size();

        //空中にいるかどうかを取得する
        IsFall = OwnerCharacter->GetCharacterMovement()->IsFalling();
    }
}
