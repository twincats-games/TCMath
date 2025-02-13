#pragma once

#include <Math/Vector.h>
#include <Math/Vector2D.h>

#include <Core/TCMathTypes.h>
#include <Core/TCMathTypeTraits.h>

namespace TC::Math
{
	/// Interpolates from Current to Target at a given speed
	/// This is framerate independent for non-linear interpolation (e.g. accelerated movement)
	/// @tparam T Type of the variable we are interpolating
	/// @param Current The current value of the variable we are interpolating
	/// @param Target The target value we are interpolating to
	/// @param DeltaTime The DeltaTime since the last interpolation/frame
	/// @param InterpSpeed How fast we are moving from Current to Target
	/// @return The next step of the interpolation from Current to Target
	template<typename T>
	[[nodiscard]] T ExponentialDecayInterp(
		const T& Current,
		const T& Target,
		float DeltaTime,
		float InterpSpeed)
	{
		// Return Current if FollowSpeed or DeltaTime <= 0
		if (InterpSpeed <= 0.0f || DeltaTime <= 0.0f)
		{
			return Current;
		}

		// Calculate the interpolation factor (Alpha) using an exponential decay formula.
		// The higher the FollowSpeed, the faster the Current value moves toward Target.
		// The longer the DeltaTime, the more influence the movement has in a single update.
		float Alpha = FMath::Exp(-InterpSpeed * DeltaTime);
		// Perform the smooth interpolation:
		// - (Current - Target) finds the difference between the two values.
		// - Multiplying by Alpha reduces the gap over time, creating a smooth transition.
		// - Adding back Target ensures that as Alpha approaches 0, Current reaches Target.
		return Target + (Current - Target) * Alpha;
	}
	
	/// Calculate the point of intersection of a line and an ovoid path
	/// @tparam T The Type of the Vector used for calculate the Intersection Location (e.g. FVector, FVector2D)
	/// @param OvoidData An instance of the Ovoid Data used to calculate the Ovoid path
	/// @param ForwardVector The Forward vector from the origin point. Used to calculate the intersecting line
	/// @param ReferenceLocation The Reference location, if not provided or Equal to T::ZeroVector the Ovoid Center will be used
	/// @param Plane When computing the Intersection point of in 2D space the user can specify a 2D plane to be considered
	/// @return The Point of intersection between the ovoid path and the line generated from origin using the forward vector
	template<typename T>
	[[nodiscard]] static T ComputeOvoidIntersection(
		const FOvoidPathData& OvoidData,
		const T& ForwardVector,
		const T& ReferenceLocation,
		E2DOvoidPlane Plane = E2DOvoidPlane::None)
	{
		// Convert FVector to the appropriate type (TVector<> or TVector2<>)
		auto ParseVector = [&](const FVector& input) -> T {
			if constexpr (IsVectorType<T>::value)
				{
				return T(input); // Keep 3D input
			}
			else if constexpr (IsVector2Type<T>::value)
				{
				// Use only 2D components depending on the selected plane
				switch (Plane)
				{
				case E2DOvoidPlane::XZ:
					return T(input.X, input.Z);
				case E2DOvoidPlane::YZ:
					return T(input.Y, input.Z);
				case E2DOvoidPlane::XY:
					return T(input.X, input.Y);
				default:
					return T{};
				}
			}
			static_assert(IsVector2Type<T>::value || IsVectorType<T>::value, "Invalid Vector Type");
			return T{};
		};

		// Convert values to match the appropriate vector type
		T OvoidCenter = ParseVector(OvoidData.Center);
		T Radii = ParseVector(OvoidData.Radii);
    
		// Determine the reference point (default to the center if not provided)
		T Origin = ReferenceLocation.IsNearlyZero() ? OvoidCenter : ReferenceLocation;

		// Normalize the forward vector (ensure it's a unit vector)
		T NormalizedForward = ForwardVector.GetSafeNormal();

		// Transform the origin point to the ovoid’s local space
		T LocalOrigin = Origin - OvoidCenter;

		// Scale the direction by the ovoid’s radii
		T ScaledForward = NormalizedForward * Radii;

		// Compute the scale factor for intersection, ensuring no division by zero
		float Denominator = NormalizedForward.X / Radii.X + NormalizedForward.Y / Radii.Y;
		if constexpr (IsVectorType<T>::value) {
			Denominator += NormalizedForward.Z / Radii.Z; // Only include Z for 3D calculations
		}

		if (FMath::IsNearlyZero(Denominator)) {
			return OvoidCenter; // Avoid division by zero by returning the center
		}

		float IntersectionScale = 1.0f / Denominator;

		// Compute the final world-space intersection
		return OvoidCenter + IntersectionScale * ScaledForward;
	}
}
