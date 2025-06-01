#include "Light.hpp"
#include "Transform.hpp"
#include "Camera.hpp"
#include "Labels.hpp"

namespace Light
{
	/*void CameraLight::setDirection(Vec3 direction)
	{
		this->direction = direction;
	}

	void CameraLight::setColor(Vec3 color)
	{
		this->color = color;
	}

	void CameraLight::setStrength(float strength)
	{
		this->strength = strength;
	}*/

	void CameraLight::start(Entity &)
	{
		setStrength(1.0f);
	}

	void CameraLight::update(Entity &entity)
	{
		Camera &camera = entity.getComponent<Camera>();
		if (camera.didChange()) {
			const Vec3 forward = camera.getForward();
			const Vec3 direction = glm::normalize(Vec3 {-forward.x, -0.6f, -forward.z});
			setDirection(direction);
		}
	}

	TransformedLight::TransformedLight(const Transform &transform, float radius)
	{
		this->position = &transform.getPosition();
		setRadius(radius);
	}
	
	/*void TransformedLight::setRadius(float radius)
	{
		this->radius = radius;
	}*/

};
