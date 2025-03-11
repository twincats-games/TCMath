# TCMath

Open Source UE5 Plugin with all extra math functions used and developed by TwinCats

Here’s a complete Markdown documentation for `ExponentialDecayInterp` and `ComputeOvoidIntersection`, including their Blueprint-exposed counterparts. Let me know if you need any refinements! 🚀  

---

# Interpolation & Ovoid Intersection Utilities

This document provides detailed information on two core utility functions: **ExponentialDecayInterp** and **ComputeOvoidIntersection**, along with their Blueprint-exposed counterparts. These functions provide advanced interpolation and geometric calculations for Unreal Engine.

## 📌 Summary

| Function Name                  | Type        | Description                                              |
|--------------------------------|------------|----------------------------------------------------------|
| `ExponentialDecayInterp`       | C++        | Smooth interpolation using exponential decay.            |
| `FExponentialDecayInterp`     | Blueprint  | Blueprint version of `ExponentialDecayInterp` for float.          |
| `VExponentialDecayInterp`     | Blueprint  | Blueprint version of `ExponentialDecayInterp` for FVector.          |
| `V2DExponentialDecayInterp`     | Blueprint  | Blueprint version of `ExponentialDecayInterp` for FVector2D.          |
| `RExponentialDecayInterp`     | Blueprint  | Blueprint version of `ExponentialDecayInterp` for FRotators.          |
| `ComputeOvoidIntersection`     | C++        | Computes where a direction vector intersects an ovoid.  |
| `VComputeOvoidIntersection`   | Blueprint  | Blueprint version of `ComputeOvoidIntersection` for FVector.        |
| `V2DComputeOvoidIntersection`   | Blueprint  | Blueprint version of `ComputeOvoidIntersection` for FVector2D.        |
| `DrawDebugOvoid`               | Debug      | Draws debug visualization of the ovoid and intersection. |

---

## 📌 Exponential Decay Interpolation

### `ExponentialDecayInterp`

#### Description
Performs an exponential decay-based interpolation between two values. Unlike standard linear interpolation, this function ensures a smooth and natural decay toward the target over time.

#### Template Signature
```cpp
template<typename T>
T ExponentialDecayInterp(
    const T& Current,
    const T& Target,
    float DeltaTime,
    float InterpSpeed
);
```

#### Parameters
- `Current` (`T`) - The current value.
- `Target` (`T`) - The target value to interpolate toward.
- `DeltaTime` (`float`) - The frame time step.
- `InterpSpeed` (`float`) - The speed factor controlling how fast the value approaches the target.

#### Returns
- `T` - The interpolated value after applying exponential decay.

#### Usage Example (C++)
```cpp
float SmoothedValue = ExponentialDecayInterp(CurrentValue, TargetValue, DeltaTime, 5.0f);
FVector SmoothedVector = ExponentialDecayInterp(CurrentLocation, TargetLocation, DeltaTime, 3.0f);
```

---

### `ExponentialDecayInterpBp` (Blueprint)

#### Description
A Blueprint-exposed version of `ExponentialDecayInterp`, allowing smooth exponential decay interpolation in Blueprints.

#### Blueprint Function Signature
```cpp
/** Smooth follow for float */
UFUNCTION(BlueprintPure, Category = "TC|Math|Interpolation")
static float FExponentialDecayInterp(float Current, float Target, float DeltaTime, float FollowSpeed);

/** Smooth follow for FVector */
UFUNCTION(BlueprintPure, Category = "TC|Math|Interpolation")
static FVector VExponentialDecayInterp(const FVector& Current, const FVector& Target, float DeltaTime, float FollowSpeed);

/** Smooth follow for FVector2D */
UFUNCTION(BlueprintPure, Category = "TC|Math|Interpolation")
static FVector2D V2DExponentialDecayInterp(const FVector2D& Current, const FVector2D& Target, float DeltaTime, float FollowSpeed);
	
/** Smooth follow for FRotator */
UFUNCTION(BlueprintPure, Category = "TC|Math|Interpolation")
static FRotator RExponentialDecayInterp(const FRotator& Current, const FRotator& Target, float DeltaTime, float FollowSpeed);
```

#### Parameters
- `Current` - The current value.
- `Target` - The desired value.
- `DeltaTime` (`float`) - Time step.
- `InterpSpeed` (`float`) - Controls the speed of interpolation.

#### Returns
- The next interpolated step value.

#### Example Blueprint Usage
1. Call **FExponentialDecayInterp** in a Blueprint event.
2. Use it to interpolate between a player's speed smoothly in a framerate indenpendent fashion.

---

## 📌 Ovoid Intersection Computation

#### Description
Calculates the intersection point of a direction vector with an **ovoid (elliptical)** path. This is useful for movement prediction, AI navigation, and physics-based interactions.

### `VComputeOvoidIntersection` (Blueprint and C++)

#### Description
A Blueprint-exposed function that computes the intersection of an ovoid path and a given direction vector.

#### Blueprint Function Signature
```cpp
// Specialization for FVector
UFUNCTION(BlueprintPure, Category = "TC|Math|Ovoid")
static FVector VComputeOvoidIntersection(
	const FOvoidPathData& OvoidData,
	const FVector& ForwardVector);

// Specialization for FVector2D
UFUNCTION(BlueprintPure, Category = "TC|Math|Ovoid")
static FVector2D V2DComputeOvoidIntersection(
	const FOvoidPathData& OvoidData,
	const FVector2D& ForwardVector);
```

#### Parameters
- `OvoidData` (`FOvoidPathData`) - Defines the ovoid properties (center, radii).
- `ForwardVector` - The direction vector.

#### Returns
- The intersection point.

#### Example Blueprint Usage
1. Define an **OvoidPathData** struct in Blueprint.
2. Use **VComputeOvoidIntersection** to determine where an object will intersect the ovoid.
3. Debug the result using `DrawDebugOvoid`.

#### Usage Example (C++)
```cpp
FVector IntersectionPoint = VComputeOvoidIntersection(OvoidData, ForwardVector);
FVector2D Intersection2D = V2DComputeOvoidIntersection(OvoidData, ForwardVector2D);
```

---

## 📌 Debug Utilities

### `DrawDebugOvoid`

#### Description
Visualizes the ovoid path in the world for debugging purposes.
The same function can be used in both C++ and Blueprint environments.

#### Function Signature
```cpp
UFUNCTION(BlueprintCallable, Category = "TC|Math|Ovoid", meta = (DevelopmentOnly))
void DrawDebugOvoid(
    const UObject* WorldContextObject,
    const FOvoidPathData& OvoidData,
    const FVector& Intersection,
    float DebugDuration,
    FColor PathColor,
    FColor IntersectionColor
);
```

#### Parameters
- `WorldContextObject` - The context for retrieving the world.
- `OvoidData` - Defines the ovoid shape.
- `Intersection` - The computed intersection point.
- `DebugDuration` - How long the debug visualization should remain.
- `PathColor` - The color of the ovoid path.
- `IntersectionColor` - The color of the intersection point.

#### Example Usage (C++)
```cpp
DrawDebugOvoid(World, OvoidData, IntersectionPoint, 5.0f, FColor::Blue, FColor::Red);
```

---
