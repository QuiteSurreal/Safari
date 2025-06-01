#pragma once

#include "Renderable.hpp"
#include "Behaviour.hpp"
#include "Transform.hpp"

namespace Light
{

	class CameraLight : public Behaviour, public Renderable::LightGlobal
	{
	public:

		//void setDirection(Vec3 direction);
		//void setColor(Vec3 color);
		//void setStrength(float strength);

		Vec3 getDirection() const override { return direction; }
		void setDirection(const Vec3& value) override { direction = value; }

		Vec3 getColor() const override { return color; }
		void setColor(const Vec3& value) override { color = value; }

		float getStrength() const override { return strength; }
		void setStrength(float value) override { strength = value; }

	private:

		void start(Entity &entity) override;
		void update(Entity &entity) override;

		Vec3 direction;
		Vec3 color;
		float strength;

	};

	class TransformedLight : public Renderable::LightPoint
	{
	public:

		TransformedLight(const Transform &transform, float radius);
		//void setRadius(float radius);

		const Vec3* getPosition() const override { return position; }
		void setPosition(const Vec3* value) override { position = value; }

		Vec3 getColor() const override { return color; }
		void setColor(const Vec3& value) override { color = value; }

		float getRadius() const override { return radius; }
		void setRadius(float value) override { radius = value; }

	
	private:

		const Vec3* position;
		Vec3 color = {};
		float radius;

	};

};
