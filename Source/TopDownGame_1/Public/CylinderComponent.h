// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Components/ShapeComponent.h"
#include "CylinderComponent.generated.h"

class FPrimitiveSceneProxy;
/**
 * 
 */
UCLASS(ClassGroup = "Collision", hidecategories = (Object, LOD, Lighting, TextureStreaming), editinlinenew, meta = (DisplayName = "Cylinder Collision", BlueprintSpawnableComponent))
class TOPDOWNGAME_1_API UCylinderComponent : public UShapeComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, export, Category = Shape, meta = (ClampMin = "0", UIMin = "0"))
	float CylinderHalfHeight;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, export, Category = Shape)
	float CylinderRadius;
	
public:
	/**
	 * Change the capsule size. This is the unscaled size, before component scale is applied.
	 * @param	InRadius : radius of end-cap hemispheres and center cylinder.
	 * @param	InHalfHeight : half-height, from capsule center to end of top or bottom hemisphere.
	 * @param	bUpdateOverlaps: if true and this shape is registered and collides, updates touching array for owner actor.
	 */
	UFUNCTION(BlueprintCallable, Category = "Components|Capsule")
	void SetCylinderSize(float InRadius, float InHalfHeight, bool bUpdateOverlaps = true);

	/**
	 * Set the capsule radius. This is the unscaled radius, before component scale is applied.
	 * If this capsule collides, updates touching array for owner actor.
	 * @param	Radius : radius of end-cap hemispheres and center cylinder.
	 * @param	bUpdateOverlaps: if true and this shape is registered and collides, updates touching array for owner actor.
	 */
	UFUNCTION(BlueprintCallable, Category = "Components|Capsule")
	void SetCylinderRadius(float Radius, bool bUpdateOverlaps = true);

	/**
	 * Set the capsule half-height. This is the unscaled half-height, before component scale is applied.
	 * If this capsule collides, updates touching array for owner actor.
	 * @param	HalfHeight : half-height, from capsule center to end of top or bottom hemisphere.
	 * @param	bUpdateOverlaps: if true and this shape is registered and collides, updates touching array for owner actor.
	 */
	UFUNCTION(BlueprintCallable, Category = "Components|Capsule")
	void SetCylinderHalfHeight(float HalfHeight, bool bUpdateOverlaps = true);

	//~ Begin UObject Interface
	virtual void PostLoad() override;
//#if WITH_EDITORONLY_DATA
//	ENGINE_API virtual void Serialize(FArchive& Ar) override;
//#endif
//#if WITH_EDITOR
//	ENGINE_API virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
//#endif // WITH_EDITOR
//	//~ End UObject Interface

	//~ Begin USceneComponent Interface
	virtual FBoxSphereBounds CalcBounds(const FTransform& LocalToWorld) const override;
	virtual void CalcBoundingCylinder(float& mCylinderRadius, float& mCylinderHalfHeight) const override;
	//~ End USceneComponent Interface

	//~ Begin UPrimitiveComponent Interface.
	virtual FPrimitiveSceneProxy* CreateSceneProxy() override;
	virtual bool IsZeroExtent() const override;
	virtual struct FCollisionShape GetCollisionShape(float Inflation = 0.0f) const override;
	virtual bool AreSymmetricRotations(const FQuat& A, const FQuat& B, const FVector& Scale3D) const override;
	//~ End UPrimitiveComponent Interface.

	//~ Begin UShapeComponent Interface
	virtual void UpdateBodySetup() override;
	//~ End UShapeComponent Interface

	/**
	 * Returns the capsule radius scaled by the component scale.
	 * @return The capsule radius scaled by the component scale.
	 */
	UFUNCTION(BlueprintCallable, Category = "Components|Cylinder")
	float GetScaledCylinderRadius() const;

	/**
	 * Returns the capsule half-height scaled by the component scale. This includes both the cylinder and hemisphere cap.
	 * @return The capsule half-height scaled by the component scale.
	 */
	UFUNCTION(BlueprintCallable, Category = "Components|Cylinder")
	float GetScaledCylinderHalfHeight() const;

	/**
	* Returns the capsule half-height minus radius (to exclude the hemisphere), scaled by the component scale.
	* From the center of the capsule this is the vertical distance along the straight cylindrical portion to the point just before the curve of top hemisphere begins.
	* @return The capsule half-height minus radius, scaled by the component scale.
	*/
	UFUNCTION(BlueprintCallable, Category = "Components|Cylinder")
	float GetScaledCylinderHalfHeight_WithoutHemisphere() const;

	/**
	 * Returns the capsule radius and half-height scaled by the component scale. Half-height includes the hemisphere end cap.
	 * @param OutRadius Radius of the capsule, scaled by the component scale.
	 * @param OutHalfHeight Half-height of the capsule, scaled by the component scale. Includes the hemisphere end cap.
	 * @return The capsule radius and half-height scaled by the component scale.
	 */
	UFUNCTION(BlueprintCallable, Category = "Components|Cylinder")
	void GetScaledCylinderSize(float& OutRadius, float& OutHalfHeight) const;

	/**
	 * Returns the capsule radius and half-height scaled by the component scale. Half-height excludes the hemisphere end cap.
	 * @param OutRadius Radius of the capsule, ignoring component scaling.
	 * @param OutHalfHeightWithoutHemisphere Half-height of the capsule, scaled by the component scale. Excludes the hemisphere end cap.
	 * @return The capsule radius and half-height scaled by the component scale.
	 */
	UFUNCTION(BlueprintCallable, Category = "Components|Cylinder")
	void GetScaledCylinderSize_WithoutHemisphere(float& OutRadius, float& OutHalfHeightWithoutHemisphere) const;

	/**
	 * Returns the capsule radius, ignoring component scaling.
	 * @return the capsule radius, ignoring component scaling.
	 */
	UFUNCTION(BlueprintCallable, Category = "Components|Cylinder")
	float GetUnscaledCylinderRadius() const;

	/**
	 * Returns the capsule half-height, ignoring component scaling. This includes the hemisphere end cap.
	 * @return The capsule radius, ignoring component scaling.
	 */
	UFUNCTION(BlueprintCallable, Category = "Components|Cylinder")
	float GetUnscaledCylinderHalfHeight() const;

	/**
	* Returns the capsule half-height minus radius (to exclude the hemisphere), ignoring component scaling. This excludes the hemisphere end cap.
	* From the center of the capsule this is the vertical distance along the straight cylindrical portion to the point just before the curve of top hemisphere begins.
	* @return The capsule half-height minus radius, ignoring component scaling.
	*/
	UFUNCTION(BlueprintCallable, Category = "Components|Cylinder")
	float GetUnscaledCylinderHalfHeight_WithoutHemisphere() const;

	/**
	 * Returns the capsule radius and half-height scaled by the component scale. Half-height includes the hemisphere end cap.
	 * @param OutRadius Radius of the capsule, scaled by the component scale.
	 * @param OutHalfHeight Half-height of the capsule, scaled by the component scale. Includes the hemisphere end cap.
	 * @return The capsule radius and half-height scaled by the component scale.
	 */
	UFUNCTION(BlueprintCallable, Category = "Components|Cylinder")
	void GetUnscaledCylinderSize(float& OutRadius, float& OutHalfHeight) const;

	/**
	 * Returns the capsule radius and half-height, ignoring component scaling. Half-height excludes the hemisphere end cap.
	 * @param OutRadius Radius of the capsule, ignoring component scaling.
	 * @param OutHalfHeightWithoutHemisphere Half-height of the capsule, scaled by the component scale. Excludes the hemisphere end cap.
	 * @return The capsule radius and half-height (excluding hemisphere end cap), ignoring component scaling.
	 */
	UFUNCTION(BlueprintCallable, Category = "Components|Cylinder")
	void GetUnscaledCylinderSize_WithoutHemisphere(float& OutRadius, float& OutHalfHeightWithoutHemisphere) const;

	/**
	 * Get the scale used by this shape. This is a uniform scale that is the minimum of any non-uniform scaling.
	 * @return the scale used by this shape.
	 */
	UFUNCTION(BlueprintCallable, Category = "Components|Cylinder")
	float GetShapeScale() const;

	// Sets the capsule size without triggering a render or physics update. This is the preferred method when initializing a component in a class constructor.
	FORCEINLINE void InitCylinderSize(float InRadius, float InHalfHeight)
	{
		CylinderRadius = FMath::Max(0.f, InRadius);
		CylinderHalfHeight = FMath::Max3(0.f, InHalfHeight, InRadius);
	}
};
