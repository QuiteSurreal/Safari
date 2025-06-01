#include "Sprite.hpp"

Sprite::Sprite(const Transform &transform, Texture texture)
{
	this->offset = Vec3 {-0.5f, 0.0f, 0.0f};
	this->texture = texture;
	this->position = &transform.getPosition();
	this->transform = &transform.getTrans();
}

