// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/TCMathTypes.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ComputeOvoidIntersectionBpUtils.generated.h"

/**
 * 
 */
UCLASS()
class TCMATH_API UComputeOvoidIntersectionBpUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/// Calculate the point of intersection of a line and an ovoid path
	/// @tparam T The Type of the Vector used for calculate the Intersection Location (e.g. FVector, FVector2D)
	/// @param OvoidData An instance of the Ovoid Data used to calculate the Ovoid path
	/// @param ForwardVector The Forward vector from the origin point. Used to calculate the intersecting line
	/// @param ReferenceLocation The Reference location, if not provided or Equal to ZeroVector the Ovoid Center will be used
	/// @return The Point of intersection between the ovoid path and the line generated from origin using the forward vector
	UFUNCTION(BlueprintPure, Category = "TC|Math|Ovoid")
	static FVector VComputeOvoidIntersection(
		const FOvoidPathData& OvoidData,
		const FVector& ForwardVector,
		const FVector& ReferenceLocation = FVector::ZeroVector);

	// Specialization for FVector2D
	UFUNCTION(BlueprintPure, Category = "TC|Math|Ovoid")
	static FVector2D V2DComputeOvoidIntersection(
		const FOvoidPathData& OvoidData,
		const FVector2D& ForwardVector,
		const FVector2D& ReferenceLocation = FVector2D::ZeroVector);

private:
	UFUNCTION(BlueprintCallable, Category = "TC|Math|Ovoid", meta = (DevelopmentOnly))
	static void DrawDebugOvoid(
		const UObject* WorldContextObject,
		const FOvoidPathData& OvoidData,
		const FVector& Intersection,
		float DebugDuration = 5.0f,
		FColor PathColor = FColor::Blue,
		FColor IntersectionColor = FColor::Red);
};
