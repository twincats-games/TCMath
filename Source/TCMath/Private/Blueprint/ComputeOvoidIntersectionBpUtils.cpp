// Fill out your copyright notice in the Description page of Project Settings.


#include "Blueprint/ComputeOvoidIntersectionBpUtils.h"

#include "Core/TCMath.h"

FVector UComputeOvoidIntersectionBpUtils::VComputeOvoidIntersection(
	const FOvoidPathData& OvoidData,
	const FVector& ForwardVector,
	const FVector& ReferenceLocation)
{
	return TC::Math::ComputeOvoidIntersection<FVector>(OvoidData, ForwardVector, ReferenceLocation);
}

FVector2D UComputeOvoidIntersectionBpUtils::V2DComputeOvoidIntersection(
	const FOvoidPathData& OvoidData,
	const FVector2D& ForwardVector,
	const FVector2D& ReferenceLocation,
	E2DOvoidPlane Plane)
{
	return TC::Math::ComputeOvoidIntersection<FVector2D>(OvoidData, ForwardVector, ReferenceLocation, Plane);
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
