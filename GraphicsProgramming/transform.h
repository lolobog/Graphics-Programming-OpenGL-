#pragma once

#include<glm.hpp>
#include<gtx/transform.hpp>

using namespace glm;




class Transform
{
public:
	Transform(const vec3& pos = vec3(0), const vec3& rot = vec3(0), const vec3& scale = vec3(1));

		~Transform();

		float* xRef = &m_position.x;
		float* yRef = &m_position.y;
		float* zRef = &m_position.z;
	
	vec3 GetPosition() { return m_position; }
	void SetPosition(vec3 _positon) { 
		m_position = _positon;
	}

	vec3 GetRoatation() { return m_rotation; }
	void SetRotation(vec3 _rotation) { m_rotation = _rotation; }

	vec3 GetScale() { return m_scale; }
	void SetScale(vec3 _scale) { m_scale = _scale; }

	Transform* GetTransform() { return this; }
	/*void SetTransform(Transform* _transform) 
	{ 
		m_position = _transform->GetPosition();
		m_rotation = _transform->GetRoatation();
		m_scale = _transform->GetScale();
	}*/

	inline mat4 GetModel()const
	{
		mat4 posMatrix = translate(m_position);
		mat4 rotXMatrix = rotate(m_rotation.x, vec3(1, 0, 0));
		mat4 rotYMatrix = rotate(m_rotation.y, vec3(0, 1, 0));
		mat4 rotZMatrix = rotate(m_rotation.z, vec3(0, 0, 1));
		mat4 scaleMatrix = scale(m_scale);
		mat4 rotMatrix = rotZMatrix * rotYMatrix * rotXMatrix;

		return posMatrix * rotMatrix * scaleMatrix;
	}



private:
	vec3 m_position;
	vec3 m_rotation;
	vec3 m_scale;
};