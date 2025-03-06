#pragma once

#include <Math/Vector.h>
#include <Math/Vector2D.h>

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
}
