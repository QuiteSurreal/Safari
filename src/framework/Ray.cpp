#include "Ray.hpp"

std::optional<float> Ray::castToPoint(Vec3 point) const
{
	const Vec3 vec = point - origin;
	if (glm::dot(vec, direction) < 0) {
		return std::nullopt;
	}
	return glm::length(glm::cross(vec, direction));
}

std::optional<Vec3> Ray::castToYPlane() const
{
	constexpr float epsilon = 1e-6f;
	const float t = std::abs(direction.y) > epsilon ? -origin.y / direction.y : 0;
	if (t > 0) {
		const Vec3 point = origin + t * direction;
		return Vec3 {point.x, 0, point.z};
	}
	return std::nullopt;
}
