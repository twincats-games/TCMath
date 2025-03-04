#pragma once

#include <type_traits>
#include <Math/Vector.h>
#include <Math/Vector2D.h>

namespace TC::Math
{
	// Type trait to check if a type is a vector type (either FVector or TVector<T>)
	template<typename T>
	struct IsVectorType : std::false_type {};  // Default is false

	// Specialization for FVector
	template<>
	struct IsVectorType<FVector> : std::true_type {};

	// Specialization for TVector<T> (any 3D vector type)
	template<typename T>
	struct IsVectorType<UE::Math::TVector<T>> : std::true_type {};

	// Type trait to check if a type is a 2D vector type (e.g., TVector2<T>)
	template<typename T>
	struct IsVector2Type : std::false_type {};  // Default is false

	// Specialization for FVector2D
	template<>
	struct IsVector2Type<FVector2D> : std::true_type {};

	// Specialization for TVector2<T> (any 2D vector type)
	template<typename T>
	struct IsVector2Type<UE::Math::TVector2<T>> : std::true_type {};
}
