#pragma once

#include "Renderable.hpp"

class Background : public Renderable::Background
{
public:

    Background(Vec3 color);

    Vec3 getColor() const override { return color; }
    void setColor(const Vec3& value) override { color = value; }

private:

    Vec3 color;

};
