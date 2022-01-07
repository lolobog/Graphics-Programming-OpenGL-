#include "transform.h"

Transform::Transform(const vec3& pos, const vec3& rot, const vec3& scale)
{
	m_position = pos;
	m_rotation = rot;
	m_scale = scale;
	int i = 0;
}

Transform::~Transform()
{
}
