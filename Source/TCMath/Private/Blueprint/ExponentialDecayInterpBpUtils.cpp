// Fill out your copyright notice in the Description page of Project Settings.


#include "Blueprint/ExponentialDecayInterpBpUtils.h"
#include <Core/TCMath.h>

#include "Config/TCMathConfig.h"
#include "Kismet/KismetMathLibrary.h"

float UExponentialDecayInterpBpUtils::FExponentialDecayInterp(float Current,float Target, float DeltaTime, float InterpSpeed)
{
	const UTCMathConfig* Settings = GetDefault<UTCMathConfig>();
	if ((Current - Target) < Settings->FloatInterpolationDeadZone)
	{
		return Target;
	}
	
	return TC::Math::ExponentialDecayInterp<float>(Current, Target, DeltaTime, InterpSpeed);
}

FVector UExponentialDecayInterpBpUtils::VExponentialDecayInterp(const FVector& Current, const FVector& Target, float DeltaTime, float InterpSpeed)
{
	const UTCMathConfig* Settings = GetDefault<UTCMathConfig>();
	if ((Current - Target).Length() < Settings->VectorInterpolationDeadZone)
	{
		return Target;
	}
	
	return TC::Math::ExponentialDecayInterp<FVector>(Current, Target, DeltaTime, InterpSpeed);
}

FVector2D UExponentialDecayInterpBpUtils::V2DExponentialDecayInterp(const FVector2D& Current, const FVector2D& Target, float DeltaTime, float InterpSpeed)
{
	const UTCMathConfig* Settings = GetDefault<UTCMathConfig>();
	if ((Current - Target).Length() < Settings->VectorInterpolationDeadZone)
	{
		return Target;
	}
	
	return TC::Math::ExponentialDecayInterp<FVector2D>(Current, Target, DeltaTime, InterpSpeed);
}

FRotator UExponentialDecayInterpBpUtils::RExponentialDecayInterp(const FRotator& Current, const FRotator& Target, float DeltaTime, float InterpSpeed)
{
	const UTCMathConfig* Settings = GetDefault<UTCMathConfig>();
	if ((Current - Target).IsNearlyZero(Settings->RotatorInterpolationDeadZone))
	{
		return Target;
	}
	
	float Alpha = TC::Math::ExponentialDecayInterp<float>(0.0f, 1.0f, InterpSpeed, DeltaTime);
	// Using RLerp prevents gimbal lock by properly interpolating rotation values
	return UKismetMathLibrary::RLerp(Current, Target, Alpha, true);
}
