#include "Camera.h"

Camera::Camera()
{
	transform.SetPosition(vec3(0, 10, -40));
	transform.SetRotation(vec3(0, 0, 0));
	transform.SetScale(vec3(1, 1, 1));
}

Camera::~Camera()
{
}
