#pragma once

#include "Component.hpp"
#include "Utils.hpp"

class Transform : virtual public Component
{
public:

	inline float getLocalScale() const { return localScale; }
	inline Vec3 getLocalPosition() const { return localPosition; }
	inline Vec3 getLocalRotation() const { return localRotation; }
	inline const Vec3 &getPosition() const { return position; }
	inline const Mat4 &getTrans() const { return trans; }

	void scale(float scale);

	void translate(Vec3 delta);

	void relocate(Vec3 value);

	void rotate(Vec3 delta);

	void setRotation(Vec3 value);

	void setParent(Transform& parent);

	void removeParent();

private:

	float localScale = 1.0f;
	Vec3 localPosition = {};
	Vec3 localRotation = {};
	Mat4 localTrans = {};

	Vec3 position = {};
	Mat4 trans = {};

	Transform* parent = nullptr;
	std::unordered_set<Transform*> children;

	void recalcLocal();

	void recalc();

	void onDetach() override;

};
