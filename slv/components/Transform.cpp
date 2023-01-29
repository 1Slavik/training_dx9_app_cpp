#include "Transform.h"

namespace components = slv::components;


components::Transform::Transform()
	: m_pParent(const_cast<Transform*>(&s_emptyTransform)),
	m_pos(0.0f, 0.0f, 0.0f)
{
}

components::Transform
components::Transform::ConstructEmptyTransform()
{
	Transform t;
	t.m_pParent = nullptr;
	return t;
}

const components::Transform
components::Transform::s_emptyTransform
	= components::Transform::ConstructEmptyTransform();