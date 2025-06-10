// Fill out your copyright notice in the Description page of Project Settings.


#include "CylinderComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(CylinderComponent)

void UCylinderComponent::SetCylinderSize(float InRadius, float InHalfHeight, bool bUpdateOverlaps)
{
	
}

void UCylinderComponent::SetCylinderRadius(float Radius, bool bUpdateOverlaps)
{
	
}

void UCylinderComponent::SetCylinderHalfHeight(float HalfHeight, bool bUpdateOverlaps)
{
	
}

void UCylinderComponent::PostLoad()
{
	
}

FBoxSphereBounds UCylinderComponent::CalcBounds(const FTransform& LocalToWorld) const
{
	return FBoxSphereBounds();
}

void UCylinderComponent::CalcBoundingCylinder(float& mCylinderRadius, float& mCylinderHalfHeight) const
{
	
}

FPrimitiveSceneProxy* UCylinderComponent::CreateSceneProxy()
{
	return nullptr;
}

 bool UCylinderComponent::IsZeroExtent() const
{
	return false;
}

FCollisionShape UCylinderComponent::GetCollisionShape(float Inflation) const
{
	return FCollisionShape();
}

bool UCylinderComponent::AreSymmetricRotations(const FQuat& A, const FQuat& B, const FVector& Scale3D) const
{
	return false;
}

void UCylinderComponent::UpdateBodySetup()
{
	
}

float UCylinderComponent::GetScaledCylinderRadius() const
{
	return float();
}

float UCylinderComponent::GetScaledCylinderHalfHeight() const
{
	return float();
}

float UCylinderComponent::GetScaledCylinderHalfHeight_WithoutHemisphere() const
{
	return float();
}

void UCylinderComponent::GetScaledCylinderSize(float& OutRadius, float& OutHalfHeight) const
{
	
}

void UCylinderComponent::GetScaledCylinderSize_WithoutHemisphere(float& OutRadius, float& OutHalfHeightWithoutHemisphere) const
{
	
}

float UCylinderComponent::GetUnscaledCylinderRadius() const
{
	return float();
}

float UCylinderComponent::GetUnscaledCylinderHalfHeight() const
{
	return float();
}

float UCylinderComponent::GetUnscaledCylinderHalfHeight_WithoutHemisphere() const
{
	return float();
}

void UCylinderComponent::GetUnscaledCylinderSize(float& OutRadius, float& OutHalfHeight) const
{
	
}

void UCylinderComponent::GetUnscaledCylinderSize_WithoutHemisphere(float& OutRadius, float& OutHalfHeightWithoutHemisphere) const
{
	
}

float UCylinderComponent::GetShapeScale() const
{
	return float();
}
