#pragma once

#include <CoreMinimal.h>
#include <TCMathTypes.generated.h>

UENUM(BlueprintType)
enum class E2DOvoidPlane : uint8
{
	None = 0,
	XY = 1,
	XZ = 2,
	YZ = 3	
};

USTRUCT(BlueprintType)
struct FOvoidPathData
{
	GENERATED_BODY()

	/** Center of the ovoid in world space */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ovoid Path")
	FVector Center = FVector::ZeroVector;

	/** Radius along each axis */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ovoid Path")
	FVector Radii = FVector(100.0f, 50.0f, 50.0f);

	/** Rotation of the ovoid (useful for non-axis-aligned ovoids) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ovoid Path")
	FRotator Rotation = FRotator::ZeroRotator;

	FOvoidPathData() {}
};
