#pragma once
#include "transform.h"

class Camera 
{
public:
	Camera();
	~Camera();

	Transform& GetTransform() { return transform; }
	void SetTransform(Transform _transform) { transform = _transform; }

	vec3 GetUpVector() { return up; }
	void SetUpVector(vec3 _up) { up = _up; }

	vec3 GetForwardVector() { return forward; }
	void SetForwardVector(vec3 _forward) { forward = _forward; }

	vec3 GetRightVector() { return right; }
	void SetRightVector(vec3 _right) { right = _right; }

	mat4 GetPerspective() { return perspective; }
	void SetPerspective(mat4 _perspective) { perspective = _perspective; }

	void SetFov(float _fov) { fov = _fov; }

	mat4 CalculateViewMatrix()
	{
		vec3 t =  vec3(0) - transform.GetPosition();
		forward = normalize(t);
		right = normalize(cross(vec3(0, 1, 0), forward));
		up = cross(forward, right);

		return glm::lookAt(transform.GetPosition(), target, up);
	}



private:
	Transform transform;
	vec3 target = vec3(0, 0, 0);
	vec3 forward;
	vec3 up;
	vec3 right;
	float fov = 45;
	float f = float(1440.0f / 1080.0f);
	mat4 perspective=glm::perspective(fov,f,1.0f,1000.0f);
};