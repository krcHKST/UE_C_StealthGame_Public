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
        //���x
        WalkSpeed = OwnerCharacter->GetVelocity().Size();

        //�󒆂ɂ��邩�ǂ������擾����
        IsFall = OwnerCharacter->GetCharacterMovement()->IsFalling();
    }
}
