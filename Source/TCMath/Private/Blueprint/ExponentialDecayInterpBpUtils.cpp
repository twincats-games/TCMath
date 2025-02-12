// Fill out your copyright notice in the Description page of Project Settings.


#include "Blueprint/ExponentialDecayInterpBpUtils.h"
#include <Core/TCMath.h>

#include "Kismet/KismetMathLibrary.h"

float UExponentialDecayInterpBpUtils::FExponentialDecayInterp(float Current,float Target, float DeltaTime, float FollowSpeed)
{
	return TC::Math::ExponentialDecayInterp<float>(Current, Target, DeltaTime, FollowSpeed);
}

FVector UExponentialDecayInterpBpUtils::VExponentialDecayInterp(const FVector& Current, const FVector& Target, float DeltaTime, float FollowSpeed)
{
	return TC::Math::ExponentialDecayInterp<FVector>(Current, Target, DeltaTime, FollowSpeed);
}

FVector2D UExponentialDecayInterpBpUtils::V2DExponentialDecayInterp(const FVector2D& Current, const FVector2D& Target, float DeltaTime, float FollowSpeed)
{
	return TC::Math::ExponentialDecayInterp<FVector2D>(Current, Target, DeltaTime, FollowSpeed);
}

FRotator UExponentialDecayInterpBpUtils::RExponentialDecayInterp(const FRotator& Current, const FRotator& Target, float DeltaTime, float FollowSpeed)
{
	float Alpha = TC::Math::ExponentialDecayInterp<float>(0.0f, 1.0f, FollowSpeed, DeltaTime);
	// Using RLerp prevents gimbal lock by properly interpolating rotation values
	return UKismetMathLibrary::RLerp(Current, Target, Alpha, true);
}
