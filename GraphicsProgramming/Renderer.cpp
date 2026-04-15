#include "Renderer.h"
#include "GameObject.h"
#include "GameObjectTypes.h"
#include "glut.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#include "ModelLoader.h"
Renderer::Renderer()
{
}

void Renderer::Render(GameObject* gameObject)
{
	// de vazut cum ma pot folosi de imgui sa arata mesaj de erorare (gameobject is null)
	if (gameObject == nullptr) return;

	glPushMatrix();
	// vom chema functiile de setare a transformarilor, materialelor, texturilor 

	ApplyTransform(gameObject->transform);
	ApplyMaterial(gameObject);
    if (gameObject->textureID != 0)
    {
        ApplyTexture(gameObject);
    }
    else
    {
		glBindTexture(GL_TEXTURE_2D, 0);
    }


	if (gameObject->type == GameObjectType::Model)
	{
		DrawModel(gameObject);
        glPopMatrix();
		return;
	}

	switch (gameObject->primitiveType)
	{
	case PrimitiveType::Cube:
		DrawCube();
		break;
	case PrimitiveType::Sphere:
		DrawSphere();
		break;
	default:
		break;
	}

	glPopMatrix();

}

void Renderer::DrawCube()
{
    glColor3f(1.0f, 1.0f, 1.0f);

    glBegin(GL_QUADS);
    {
        // Front face 
        glNormal3f(0.0f, 0.0f, 1.0f);

        glTexCoord2f(0.0f, 1.0f);
        glVertex3f(-0.5f, 0.5f, 0.5f);

        glTexCoord2f(1.0f, 1.0f);
        glVertex3f(0.5f, 0.5f, 0.5f);

        glTexCoord2f(1.0f, 0.0f);
        glVertex3f(0.5f, -0.5f, 0.5f);

        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(-0.5f, -0.5f, 0.5f);

        // Back face 
        glNormal3f(0.0f, 0.0f, -1.0f);

        glTexCoord2f(0.0f, 1.0f);
        glVertex3f(0.5f, 0.5f, -0.5f);

        glTexCoord2f(1.0f, 1.0f);
        glVertex3f(-0.5f, 0.5f, -0.5f);

        glTexCoord2f(1.0f, 0.0f);
        glVertex3f(-0.5f, -0.5f, -0.5f);

        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(0.5f, -0.5f, -0.5f);

        // Left face 
        glNormal3f(-1.0f, 0.0f, 0.0f);

        glTexCoord2f(0.0f, 1.0f);
        glVertex3f(-0.5f, 0.5f, -0.5f);

        glTexCoord2f(1.0f, 1.0f);
        glVertex3f(-0.5f, 0.5f, 0.5f);

        glTexCoord2f(1.0f, 0.0f);
        glVertex3f(-0.5f, -0.5f, 0.5f);

        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(-0.5f, -0.5f, -0.5f);

        // Right 
        glNormal3f(1.0f, 0.0f, 0.0f);

        glTexCoord2f(0.0f, 1.0f);
        glVertex3f(0.5f, 0.5f, 0.5f);

        glTexCoord2f(1.0f, 1.0f);
        glVertex3f(0.5f, 0.5f, -0.5f);

        glTexCoord2f(1.0f, 0.0f);
        glVertex3f(0.5f, -0.5f, -0.5f);

        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(0.5f, -0.5f, 0.5f);

        // Top face 
        glNormal3f(0.0f, 1.0f, 0.0f);

        glTexCoord2f(0.0f, 1.0f);
        glVertex3f(-0.5f, 0.5f, -0.5f);

        glTexCoord2f(1.0f, 1.0f);
        glVertex3f(0.5f, 0.5f, -0.5f);

        glTexCoord2f(1.0f, 0.0f);
        glVertex3f(0.5f, 0.5f, 0.5f);

        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(-0.5f, 0.5f, 0.5f);

        // Bottom face 
        glNormal3f(0.0f, -1.0f, 0.0f);

        glTexCoord2f(0.0f, 1.0f);
        glVertex3f(0.5f, -0.5f, 0.5f);

        glTexCoord2f(1.0f, 1.0f);
        glVertex3f(0.5f, -0.5f, -0.5f);

        glTexCoord2f(1.0f, 0.0f);
        glVertex3f(-0.5f, -0.5f, -0.5f);

        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(-0.5f, -0.5f, 0.5f);
    }
    glEnd();
}

void Renderer::DrawSphere()
{
}

void Renderer::DrawModel(GameObject* gameObject)
{
	if (gameObject->model != nullptr)
	{
		gameObject->model->RenderModel();
	}
}

void Renderer::ApplyTransform(Transform objectTransform)
{
	glTranslatef(objectTransform.position.x, objectTransform.position.y, objectTransform.position.z);
    glRotatef(objectTransform.rotation.x, 1, 0, 0);
    glRotatef(objectTransform.rotation.y, 0, 1, 0);
    glRotatef(objectTransform.rotation.z, 0, 0, 1);
	glScalef(objectTransform.scale.x, objectTransform.scale.y, objectTransform.scale.z);
}

void Renderer::ApplyMaterial(const GameObject* gameObject)
{
    GLfloat ambient[] =
    {
        gameObject->material.ambient.x,
        gameObject->material.ambient.y,
        gameObject->material.ambient.z,
        1.0f
    };

    GLfloat diffuse[] =
    {
        gameObject->material.diffuse.x,
        gameObject->material.diffuse.y,
        gameObject->material.diffuse.z,
        1.0f
    };

    GLfloat specular[] =
    {
        gameObject->material.specular.x,
        gameObject->material.specular.y,
        gameObject->material.specular.z,
        1.0f
    };

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, gameObject->material.shininess);
}

void Renderer::ApplyLights(const std::vector<Light*>& lights, const Vector3& globalAmbient)
{
    GLfloat globalAmbientValues[] =
    {
        globalAmbient.x, globalAmbient.y, globalAmbient.z, 1.0f
    };

    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmbientValues);

    for (int i = 0; i < 8; i++)
    {
        glDisable(GL_LIGHT0 + i);
    }

    for (int i = 0; i < (int)lights.size() && i < 8; i++)
    {
        Light* light = lights[i];
        if (light == nullptr || !light->active)
            continue;

        GLenum lightId = GL_LIGHT0 + i;
        glEnable(lightId);

        GLfloat ambient[] = { light->ambient.x, light->ambient.y, light->ambient.z, 1.0f };
        GLfloat diffuse[] = { light->diffuse.x, light->diffuse.y, light->diffuse.z, 1.0f };
        GLfloat specular[] = { light->specular.x, light->specular.y, light->specular.z, 1.0f };

        glLightfv(lightId, GL_AMBIENT, ambient);
        glLightfv(lightId, GL_DIFFUSE, diffuse);
        glLightfv(lightId, GL_SPECULAR, specular);

        if (light->type == LightType::Directional)
        {
            GLfloat position[] =
            {
                -light->direction.x,
                -light->direction.y,
                -light->direction.z,
                0.0f
            };
            glLightfv(lightId, GL_POSITION, position);
        }
        else if (light->type == LightType::Point)
        {
            GLfloat position[] =
            {
                light->position.x,
                light->position.y,
                light->position.z,
                1.0f
            };

            glLightfv(lightId, GL_POSITION, position);
            glLightf(lightId, GL_CONSTANT_ATTENUATION, light->constantAttenuation);
            glLightf(lightId, GL_LINEAR_ATTENUATION, light->linearAttenuation);
            glLightf(lightId, GL_QUADRATIC_ATTENUATION, light->quadraticAttenuation);
            glLightf(lightId, GL_SPOT_CUTOFF, 180.0f);
        }
        else if (light->type == LightType::Spotlight)
        {
            GLfloat position[] =
            {
                light->position.x,
                light->position.y,
                light->position.z,
                1.0f
            };

            GLfloat direction[] =
            {
                light->direction.x,
                light->direction.y,
                light->direction.z
            };

            glLightfv(lightId, GL_POSITION, position);
            glLightfv(lightId, GL_SPOT_DIRECTION, direction);
            glLightf(lightId, GL_SPOT_CUTOFF, light->cutoff);
            glLightf(lightId, GL_SPOT_EXPONENT, light->exponent);

            glLightf(lightId, GL_CONSTANT_ATTENUATION, light->constantAttenuation);
            glLightf(lightId, GL_LINEAR_ATTENUATION, light->linearAttenuation);
            glLightf(lightId, GL_QUADRATIC_ATTENUATION, light->quadraticAttenuation);
        }
    }
}

void Renderer::ApplyTexture(GameObject* gameObject)
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, gameObject->textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void Renderer::LoadTexture()
{
}

void Renderer::RenderSphereAtLightLocation(Light* light)
{
    if (light == nullptr) return;
    if (!light->visible) return;

    glPushAttrib(GL_ENABLE_BIT | GL_CURRENT_BIT);

    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);

    glColor3f(1.0f, 1.0f, 0.0f); // galben

    glPushMatrix();
    glTranslatef(light->position.x, light->position.y, light->position.z);
    glutSolidSphere(0.25f, 12, 12);

    glPopMatrix();

    glPopAttrib();
}