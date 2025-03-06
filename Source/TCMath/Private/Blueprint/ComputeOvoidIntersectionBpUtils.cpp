// Fill out your copyright notice in the Description page of Project Settings.


#include "Blueprint/ComputeOvoidIntersectionBpUtils.h"


FVector UComputeOvoidIntersectionBpUtils::VComputeOvoidIntersection(
	const FOvoidPathData& OvoidData,
	const FVector& ForwardVector,
	const FVector& ReferenceLocation)
{
	FVector OvoidCenter = OvoidData.Center;
	FVector Radii = OvoidData.Radii;
	const FVector ReferenceVector = ReferenceLocation.IsNearlyZero() ? OvoidCenter : ReferenceLocation;

	// 1. Convert Forward Vector to an angle
	float Theta = FMath::Atan2(ForwardVector.Y, ForwardVector.X);

	// 2. Compute the first intersection point (C)
	FVector C;
	C.X = OvoidCenter.X + Radii.X * FMath::Cos(Theta);
	C.Y = OvoidCenter.Y + Radii.Y * FMath::Sin(Theta);
	C.Z = OvoidCenter.Z; // Keep Z unchanged if working in 2D space

	// If the reference location is the same as the ovoid center, return C
	if (ReferenceVector.Equals(OvoidCenter, 0.01f))
	{
		return C;
	}

	// 3. Compute triangle sides
	FVector AC = C - OvoidCenter;
	FVector BC = C - ReferenceVector;

	// 4. Compute angle between AC and BC using the dot product formula
	float DotProduct = FVector::DotProduct(AC.GetSafeNormal(), BC.GetSafeNormal());
	float AngleAC_BC = FMath::Acos(FMath::Clamp(DotProduct, -1.0f, 1.0f));

	// 5. Adjust the angle based on the reference location's position
	float FinalTheta = Theta + AngleAC_BC; // or Theta - AngleAC_BC depending on placement

	// 6. Compute the final intersection point
	FVector FinalIntersection;
	FinalIntersection.X = OvoidCenter.X + Radii.X * FMath::Cos(FinalTheta);
	FinalIntersection.Y = OvoidCenter.Y + Radii.Y * FMath::Sin(FinalTheta);
	FinalIntersection.Z = C.Z;

	return FinalIntersection;
}

FVector2D UComputeOvoidIntersectionBpUtils::V2DComputeOvoidIntersection(
	const FOvoidPathData& OvoidData,
	const FVector2D& ForwardVector,
	const FVector2D& ReferenceLocation)
{
	// Convert FVector2D to FVector (Z = 0)
	FVector ForwardVector3D(ForwardVector.X, ForwardVector.Y, 0.0f);
	FVector ReferenceLocation3D(ReferenceLocation.X, ReferenceLocation.Y, 0.0f);
	// Call the 3D function
	FVector WorldIntersection = VComputeOvoidIntersection(OvoidData, ForwardVector3D, ReferenceLocation3D);
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
