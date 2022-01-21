#pragma once
#include "Camera.h"
#include <glew.h>
class LightBase
{
public:
	LightBase();
	~LightBase();
	void Draw(Camera* Cam);
	Transform& GetTransform() { 
		return m_Transform; }
	vec3 M_Color;
private:
	Transform m_Transform;
	
};