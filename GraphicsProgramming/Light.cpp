#include "Light.h"

Light::Light()
{
	name = "Light";
	type = LightType::Point;

	active = true;
	visible = true;

	position = Vector3(0.0f, 0.0f, 0.0f);
	direction = Vector3(0.0f, -1.0f, 0.0f);

	ambient = Vector3(0.1f, 0.1f, 0.1f);
	diffuse = Vector3(1.0f, 1.0f, 1.0f);
	specular = Vector3(1.0f, 1.0f, 1.0f);

	constantAttenuation = 1.0f;
	linearAttenuation = 0.09f;
	quadraticAttenuation = 0.032f;

	cutoff = 20.0f;
	exponent = 16.0f;
}
