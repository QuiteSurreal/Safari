#pragma once

#include "Behaviour.hpp"
#include "Camera.hpp"

class FaceCamera : public Behaviour
{

private:
	void start(Entity &entity) override;
	void update(Entity &entity) override;
	void rotate(Camera &camera, Transform &trans);
};
