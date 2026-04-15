#include "Material.h"

Material::Material()
{
	ambient = Vector3(0.2f, 0.2f, 0.2f);
	diffuse = Vector3(0.8f, 0.8f, 0.8f);
	specular = Vector3(1.0f, 1.0f, 1.0f);
	shininess = 32.0f;
}
