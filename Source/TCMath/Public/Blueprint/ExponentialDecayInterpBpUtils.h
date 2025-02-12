// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ExponentialDecayInterpBpUtils.generated.h"

/**
 * 
 */
UCLASS()
class TCMATH_API UExponentialDecayInterpBpUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/** Smooth follow for float */
	UFUNCTION(BlueprintPure, Category = "TC|Math|Interpolation")
	static float FExponentialDecayInterp(float Current, float Target, float DeltaTime, float FollowSpeed);

	/** Smooth follow for FVector */
	UFUNCTION(BlueprintPure, Category = "TC|Math|Interpolation")
	static FVector VExponentialDecayInterp(const FVector& Current, const FVector& Target, float DeltaTime, float FollowSpeed);

	/** Smooth follow for FVector2D */
	UFUNCTION(BlueprintPure, Category = "TC|Math|Interpolation")
	static FVector2D V2DExponentialDecayInterp(const FVector2D& Current, const FVector2D& Target, float DeltaTime, float FollowSpeed);
	
	/** Smooth follow for FRotator */
	UFUNCTION(BlueprintPure, Category = "TC|Math|Interpolation")
	static FRotator RExponentialDecayInterp(const FRotator& Current, const FRotator& Target, float DeltaTime, float FollowSpeed);
};
