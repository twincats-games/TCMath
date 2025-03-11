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

	// Compute the normal of the plane in which the ovoid lies.
	// The plane is defined by the center of the ovoid and the intersection point.
	FVector Normal = (Intersection - OvoidData.Center).GetSafeNormal();
	if (Normal.IsNearlyZero())
	{
		return; // Avoid invalid normal (e.g., when intersection == center)
	}

	// Compute two perpendicular vectors (AxisX, AxisY) that form a basis for the plane.
	// These vectors define the local coordinate system where we will draw the ovoid.
	FVector AxisX, AxisY;
	FVector::CreateOrthonormalBasis(AxisX, AxisY, Normal);

	// Define the number of segments for approximating the ovoid path as a series of line segments.
	const int NumSegments = 32;

	// Compute the first point on the ovoid path
	FVector LastPoint = OvoidData.Center + (AxisX * OvoidData.Radii.X);

	// Iterate through the defined number of segments to approximate the ovoid path
	for (int i = 1; i <= NumSegments; ++i)
	{
		// Compute the angle for the next segment
		float Angle = (2.0f * PI * i) / NumSegments;

		// Compute the next point using ellipse parametric equations, projected onto the plane
		FVector NextPoint = OvoidData.Center + 
		                    (AxisX * (OvoidData.Radii.X * FMath::Cos(Angle))) + 
		                    (AxisY * (OvoidData.Radii.Y * FMath::Sin(Angle)));

		// Draw a debug line between the previous and the current segment
		DrawDebugLine(World, LastPoint, NextPoint, PathColor, false, DebugDuration, 0, 2.0f);
		
		// Update the last point for the next iteration
		LastPoint = NextPoint;
	}

	// Draw a small sphere at the intersection point for better visualization
	DrawDebugSphere(World, Intersection, 10.0f, 12, IntersectionColor, false, DebugDuration, 0, 2.0f);
}
