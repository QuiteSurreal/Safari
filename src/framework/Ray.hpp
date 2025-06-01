#pragma once

#include <optional>

#include "Utils.hpp"

struct Ray
{
	
	Vec3 origin;
	Vec3 direction;

	std::optional<float> castToPoint(Vec3 point) const;
	std::optional<Vec3> castToYPlane() const;

};
