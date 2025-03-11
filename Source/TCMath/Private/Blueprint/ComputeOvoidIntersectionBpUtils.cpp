// Fill out your copyright notice in the Description page of Project Settings.


#include "Blueprint/ComputeOvoidIntersectionBpUtils.h"


FVector UComputeOvoidIntersectionBpUtils::VComputeOvoidIntersection(
	const FOvoidPathData& OvoidData,
	const FVector& ForwardVector)
{
	// Extract Ovoid properties
	const FVector& OvoidCenter = OvoidData.Center;
	const FVector& Radii = OvoidData.Radii;
	const FRotator& OvoidRotation = OvoidData.Rotation;

	// Step 1: Convert ForwardVector to Ovoid's Local Space
	// Get the inverse rotation to transform world-space vectors into local ovoid space
	FQuat InverseRotation = OvoidRotation.Quaternion().Inverse();
	FVector LocalForward = InverseRotation.RotateVector(ForwardVector.GetSafeNormal());
	
	// Step 2: Convert the direction to an ellipse-friendly form
	FVector NormalizedDir = FVector(LocalForward.X / Radii.X, LocalForward.Y / Radii.Y, 0.0f);
	NormalizedDir.Normalize(); // Re-normalize after applying inverse scaling

	// Step 3: Compute intersection in local space
	FVector LocalIntersection = FVector(
		Radii.X * NormalizedDir.X,
		Radii.Y * NormalizedDir.Y,
		0.0f // Keep in the ovoid’s 2D plane
	);

	// Step 3: Convert back to World Space
	FVector WorldIntersection = OvoidCenter + OvoidRotation.RotateVector(LocalIntersection);
	return WorldIntersection;
}

FVector2D UComputeOvoidIntersectionBpUtils::V2DComputeOvoidIntersection(
	const FOvoidPathData& OvoidData,
	const FVector2D& ForwardVector)
{
	// Convert FVector2D to FVector (Z = 0)
	FVector ForwardVector3D(ForwardVector.X, ForwardVector.Y, 0.0f);
	// Call the 3D function
	FVector WorldIntersection = VComputeOvoidIntersection(OvoidData, ForwardVector3D);
	// Convert back to FVector2D
	return FVector2D(WorldIntersection.X, WorldIntersection.Y);
}

void UComputeOvoidIntersectionBpUtils::DrawDebugOvoid(
	const UObject* WorldContextObject,
	const FOvoidPathData& OvoidData,
	const FVector& Intersection,
	float DebugDuration,
	FColor PathColor,
	FColor IntersectionColor)
{
	// Get the world from the context object
	if (!WorldContextObject)
	{
		return;
	}

	UWorld* World = WorldContextObject->GetWorld();
	if (!World)
	{
		return;
	}

	// Compute the plane normal using the intersection point relative to the center
	FVector Normal = (Intersection - OvoidData.Center).GetSafeNormal();
    
	// If the intersection is the center, return (avoid invalid normal)
	if (Normal.IsNearlyZero())
	{
		return;
	}

	// Get the ovoid's local X and Y axes, considering its rotation
	FVector AxisX = OvoidData.Rotation.RotateVector(FVector(1, 0, 0)) * OvoidData.Radii.X;
	FVector AxisY = OvoidData.Rotation.RotateVector(FVector(0, 1, 0)) * OvoidData.Radii.Y;

	// Define number of segments
	const int NumSegments = 32;
	const float AngleStep = 360.0f / NumSegments;

	// Compute the first point using the ovoid transformation
	FVector LastPoint = OvoidData.Center + AxisX;

	for (int i = 1; i <= NumSegments; ++i)
	{
		// Compute the next point using rotation in the local ovoid space
		float AngleRad = FMath::DegreesToRadians(AngleStep * i);
		FVector NextPoint = OvoidData.Center + AxisX * FMath::Cos(AngleRad) + AxisY * FMath::Sin(AngleRad);

		// Draw the segment
		DrawDebugLine(World, LastPoint, NextPoint, PathColor, false, DebugDuration, 0, 2.0f);

		// Update for the next segment
		LastPoint = NextPoint;
	}

	// Draw a small sphere at the intersection point for better visualization
	DrawDebugSphere(World, Intersection, 10.0f, 12, IntersectionColor, false, DebugDuration, 0, 2.0f);
}
