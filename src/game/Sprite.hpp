#pragma once

#include "Renderable.hpp"
#include "Texture.hpp"
#include "Transform.hpp"

class Sprite : public Renderable::Sprite
{
public:

	Sprite(const Transform &transform, Texture texture);

	const Vec3* getPosition() const override { return position; }
	void setPosition(const Vec3* value) override { position = value; }

	const Mat4* getTransform() const override { return transform; }
	void setTransform(const Mat4* value) override { transform = value; }

	Vec3 getOffset() const override { return offset; }
	void setOffset(const Vec3& value) override { offset = value; }

	Texture getTexture() const override { return texture; }
	void setTexture(const Texture& value) override { texture = value; }

	float getOpacity() const override { return opacity; }
	void setOpacity(float value) override { opacity = value; }


private:

	const Vec3* position;
	const Mat4* transform;
	Vec3 offset;
	Texture texture;
	float opacity = {};

};
