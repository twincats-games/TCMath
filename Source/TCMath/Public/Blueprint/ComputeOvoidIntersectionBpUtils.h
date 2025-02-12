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
	// Specialization for FVector
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
		const FVector2D& ReferenceLocation = FVector2D::ZeroVector,
		E2DOvoidPlane Plane = E2DOvoidPlane::XY);

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
