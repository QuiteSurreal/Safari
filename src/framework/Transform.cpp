#include "Transform.hpp"

void Transform::translate(Vec3 delta)
{
	localPosition += delta;
	recalcLocal();
}

void Transform::relocate(Vec3 value)
{
	localPosition = value;
	recalcLocal();
}

void Transform::scale(float scale)
{
	localScale = scale;
	recalcLocal();
}

void Transform::rotate(Vec3 delta)
{
	localRotation += delta;
	recalcLocal();
}

void Transform::setRotation(Vec3 value)
{
	localRotation = value;
	recalcLocal();
}

void Transform::setParent(Transform& parent)
{
	this->parent = &parent;
	parent.children.insert(this);
	recalc();
}

void Transform::removeParent()
{
	parent = nullptr;
	recalc();
}

void Transform::recalcLocal()
{
	localTrans = Mat4 {1.0f};
	localTrans = glm::translate(localTrans, localPosition);
	localTrans = glm::rotate(localTrans, localRotation.z, {0.0f, 0.0f, 1.0f });
	localTrans = glm::rotate(localTrans, localRotation.y, {0.0f, 1.0f, 0.0f});
	localTrans = glm::rotate(localTrans, localRotation.x, {1.0f, 0.0f, 0.0f});
	localTrans = glm::scale(localTrans, Vec3 {localScale});
	recalc();
}

void Transform::recalc()
{
	if (parent) {
		trans = parent->getTrans() * localTrans;
	}
	else {
		trans = localTrans;
	}

	position = trans[3];

	for (Transform* child : children) {
		child->recalc();
	}
}

void Transform::onDetach()
{
	if (parent) {
		parent->children.erase(this);
	}
	for (Transform* child : children) {
		child->removeParent();
	}
}
