#include "Camera.hpp"
#include "Input.hpp"

void Camera::place(Vec3 center, Vec2 rot, float distance)
{
	this->center = center;
	this->rot = rot;
	this->distance = distance;
	calcViewPos();
	calcProj();
}

void Camera::changeCenter(Vec2 delta)
{
	const Vec3 forward = getForward();
	const Vec3 right = {forward.z, 0.0f, -forward.x};

	center += delta.y * speed * forward;
	center += delta.x * speed * right;

	calcViewPos();
}

void Camera::changeRotation(Vec2 delta)
{
	rot += delta * sensitivity * 2.f;

	while (rot.y < 0.0f) {
		rot.y += 2 * PI_F;
	}
	while (rot.y >= 2 * PI_F) {
		rot.y -= 2 * PI_F;
	}

	if (rot.x > -rotationLimit) {
		rot.x = -rotationLimit;
	}
	if (rot.x < -PI_F / 2 + rotationLimit) {
		rot.x = -PI_F / 2 + rotationLimit;
	}

	calcViewPos();
}

void Camera::changeDistance(float delta)
{
	distance += delta * sensitivity * 10.0f; // TODO
	distance = glm::clamp(distance, distanceMin, distanceMax);

	calcViewPos();
}

void Camera::resize()
{
	calcProj();
}

void Camera::calcViewPos()
{
	view = Mat4 {1.0f};
	view = glm::translate(view, center);
	view = glm::rotate(view, rot.y, {0.0f, 1.0f, 0.0f});
	view = glm::rotate(view, rot.x, {1.0f, 0.0f, 0.0f});
	view = glm::translate(view, {0.0f, 0.0f, distance});
	
	pos = view * Vec4 {Vec3 {0.0f}, 1.0f};
	
	view = glm::inverse(view);
	
	changed = true;
}

void Camera::calcProj()
{
	proj = glm::perspective(fov, Input::getWindowRatio(), zNear, zFar);
	
	changed = true;
}

Vec3 Camera::getForward() const
{
	return {cosf(rot.y + PI_2_F), 0.0f, -sinf(rot.y + PI_2_F)};
}

Ray Camera::getRay(Ivec2 screenPosition) const
{
	const Ivec2 windowSize = Input::getWindowSize();
	const Vec2 ndc = Vec2 {(float) screenPosition.x / windowSize.x, 1.0f - (float) screenPosition.y / windowSize.y} * 2.0f - 1.0f;
	const Mat4 invViewProj = glm::inverse(proj * view);
	const Vec4 rayNdcMin {ndc, -1.0f, 1.0f};
	const Vec4 rayNdcMax {ndc, +1.0f, 1.0f};
	Vec4 rayMin = invViewProj * rayNdcMin;
	Vec4 rayMax = invViewProj * rayNdcMax;
	rayMin /= rayMin.w;
	rayMax /= rayMax.w;
	const Vec3 origin = rayMin;
	const Vec3 direction = glm::normalize(rayMax - rayMin);
	return {origin, direction};
}

void Camera::start(Entity &)
{
	place(
		Vec3 {0.0f},
		Vec2 {-PI_F / 8 - 0.1f, PI_F / 4.0f},
		(Camera::distanceMin + Camera::distanceMax) / 2
	);
}

void Camera::earlyUpdate(Entity &)
{
	if (!enabled) {
		return;
	}
	changed = false;
	if (Input::didMouseMove()) {
		const Vec2 delta = Input::getMouseMoveDelta();
		if (Input::isMouseButtonPressed(Input::MouseButton::Right)) {
			const Vec2 offset = -Vec2 {delta.y, delta.x} * Input::getDeltaTime();
			changeRotation(offset);
		}
		if (Input::isMouseButtonPressed(Input::MouseButton::Left)) {
			const Vec2 offset = Vec2 {delta.x, delta.y} * Input::getDeltaTime();
			changeCenter(offset);
		}
	}
	if (Input::didMouseScroll()) {
		float delta = Input::getMouseScrollDelta();
		changeDistance(delta);
	}
	if (Input::didWindowSizeChange()) {
		resize();
	}
	if (init) {
		init = false;
		changed = true;
	}
}
