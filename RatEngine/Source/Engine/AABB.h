#pragma once
#include "Vector3.h"
#include "Matrix4x4.h"

class AABB
{
public:
	inline AABB() {}
	inline AABB(const Vector3& minExtents, const Vector3& maxExtents);
	AABB(Vector3* points, uint32_t amt);
	AABB(float* points, uint32_t amt, uint32_t stride = 0);

	bool intersectRay(const Vector3& start, const Vector3& rayDir, float& point1, float& point2) const;
	bool intersectLine(const Vector3& start, const Vector3& end) const;
	inline bool intersects(const AABB& other) const;
	AABB transform(const Matrix4x4& transform) const;

	inline AABB expand(float distance) const;
	inline AABB expand(const Vector3& amt) const;
	inline AABB moveTo(const Vector3& destination) const;
	inline Vector3 getCenter() const;
	inline Vector3 getExtents() const;
	inline Vector3 getMinExtents() const;
	inline Vector3 getMaxExtents() const;
	inline void getCenterAndExtents(Vector3& center, Vector3& extents) const;
	inline float getVolume() const;
	inline AABB overlap(const AABB& other) const;
	inline bool contains(const Vector3& point) const;
	inline bool contains(const AABB& other) const;

	inline AABB translate(const Vector3& amt) const;
	inline AABB scaleFromCenter(const Vector3& amt) const;
	inline AABB scaleFromOrigin(const Vector3& amt) const;

	inline AABB addPoint(const Vector3& other) const;
	inline AABB addAABB(const AABB& other) const;

	inline bool operator==(const AABB& other) const;
	inline bool operator!=(const AABB& other) const;
	inline bool equals(const AABB& other, float errorMargin = 1.e-4f) const;
private:
	Vector3 extents[2];
};

