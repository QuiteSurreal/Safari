#pragma once

#include "Utils.hpp"
#include "Ray.hpp"
#include "Renderable.hpp"
#include "Behaviour.hpp"

class Camera : public Behaviour, public Renderable::ViewPoint
{
	friend class JeepIntegrationTest_RotateFacesCameraAndFlipsWhenDestinationIsBehind_Test;
	friend class DroneIntegrationTest_RotateFacesCameraAndFlipsWhenDestinationIsBehind_Test;
	friend class AirshipIntegrationTest_RotateFacesCameraAndFlipsWhenDestinationIsBehind_Test;
	friend class DroneStationIntegrationTest_RotateFacesCameraAndFlipsWhenDestinationIsBehind_Test;
	friend class HunterIntegrationTest_RotateFacesCameraAndFlipsWhenDestinationIsBehind_Test;
	friend class HunterHutIntegrationTest_RotateFacesCameraAndFlipsWhenDestinationIsBehind_Test;
	friend class PoacherIntegrationTest_RotateFacesCameraAndFlipsWhenDestinationIsBehind_Test;
	friend class SecurityCameraIntegrationTest_RotateFacesCameraAndFlipsWhenDestinationIsBehind_Test;

public:

	Camera() = default;

	static constexpr float speed = 1.f;
	static constexpr float sensitivity = 0.075f;
	static constexpr float fov = 45.0f;
	
	static constexpr float distanceMin = 1.5f;
	static constexpr float distanceMax = 15.0f;
	static constexpr float rotationLimit = 0.1f;

	static constexpr float zNear = 0.1f;
	static constexpr float zFar = 250.0f;

	inline Vec3 getPos() const { return pos; }
	inline Vec3 getCenter() const { return center; }
	inline Vec2 getRot() const { return rot; }
	inline float getDistance() const { return distance; }

	void place(Vec3 center, Vec2 rot, float distance);
	void changeCenter(Vec2 delta);
	void changeRotation(Vec2 delta);
	void changeDistance(float delta);
	void resize();

	Vec3 getForward() const;
	Ray getRay(Ivec2 screenPosition) const;

	inline bool didChange() { return changed; }

	inline void enable() { enabled = true; }
	inline void disable() { enabled = false; }

private:

	bool changed = false, init = true, enabled;

	Vec3 center;
	Vec2 rot;
	float distance;

	void calcViewPos();
	void calcProj();

	void start(Entity &entity) override;
	void earlyUpdate(Entity &entity) override;

};
