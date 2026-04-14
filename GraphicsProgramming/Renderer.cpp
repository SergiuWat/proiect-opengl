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

void Renderer::ApplyMaterial()
{
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
