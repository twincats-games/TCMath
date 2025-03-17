// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "TCMathConfig.generated.h"

/**
 * 
 */
UCLASS(Config=Engine, DefaultConfig, meta=(DisplayName="TCMath Settings"))
class TCMATH_API UTCMathConfig : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(Config, EditAnywhere, Category="Exponential Decay Interpolation|Dead Zone", meta=(DisplayName="Float"))
	float FloatInterpolationDeadZone = 0.1f;

	UPROPERTY(Config, EditAnywhere, Category="Exponential Decay Interpolation|Dead Zone", meta=(DisplayName="Vector"))
	float VectorInterpolationDeadZone = 0.5f;
	
	UPROPERTY(Config, EditAnywhere, Category="Exponential Decay Interpolation|Dead Zone", meta=(DisplayName="Rotator"))
	float RotatorInterpolationDeadZone = 1.0f;
};
