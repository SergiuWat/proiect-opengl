#include "Skybox.h"
#include "SOIL.h"

Skybox::Skybox()
{

}

void Skybox::loadTexture()
{
    frontSky = SOIL_load_OGL_texture("gfx/front.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
    backSky = SOIL_load_OGL_texture("gfx/back.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
    bottomSky = SOIL_load_OGL_texture("gfx/bottom.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
    leftSky = SOIL_load_OGL_texture("gfx/left.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
    rightSky = SOIL_load_OGL_texture("gfx/right.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
    topSky = SOIL_load_OGL_texture("gfx/top.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);

    GLuint textures[] = { frontSky, backSky, bottomSky, leftSky, rightSky, topSky };

    for (GLuint tex : textures)
    {
        glBindTexture(GL_TEXTURE_2D, tex);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }

    glBindTexture(GL_TEXTURE_2D, 0);
}

void Skybox::render(float x, float y, float z)
{
    float size = 50.0f;

    glPushMatrix();

    glTranslatef(x, y, z);

    glDepthMask(GL_FALSE);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glDisable(GL_CULL_FACE);

    glEnable(GL_TEXTURE_2D);
    glColor3f(1.0f, 1.0f, 1.0f);

    // FRONT
    glBindTexture(GL_TEXTURE_2D, frontSky);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-size, size, size);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(size, size, size);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(size, -size, size);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-size, -size, size);
    glEnd();

    // BACK
    glBindTexture(GL_TEXTURE_2D, backSky);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(size, size, -size);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-size, size, -size);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-size, -size, -size);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(size, -size, -size);
    glEnd();

    // LEFT
    glBindTexture(GL_TEXTURE_2D, leftSky);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-size, size, -size);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-size, size, size);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-size, -size, size);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-size, -size, -size);
    glEnd();

    // RIGHT
    glBindTexture(GL_TEXTURE_2D, rightSky);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(size, size, size);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(size, size, -size);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(size, -size, -size);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(size, -size, size);
    glEnd();

    // TOP
    glBindTexture(GL_TEXTURE_2D, topSky);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-size, size, -size);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(size, size, -size);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(size, size, size);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-size, size, size);
    glEnd();

    // BOTTOM
    glBindTexture(GL_TEXTURE_2D, bottomSky);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-size, -size, size);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(size, -size, size);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(size, -size, -size);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-size, -size, -size);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, 0);

    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glEnable(GL_LIGHTING);

    glPopMatrix();
}