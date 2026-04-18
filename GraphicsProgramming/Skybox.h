#pragma once
#include "glut.h"

class Skybox
{
public:

    Skybox();
    void loadTexture();
    void render(float x, float y, float z);

private:
    GLuint frontSky;
    GLuint backSky;
    GLuint bottomSky;
    GLuint leftSky;
    GLuint rightSky;
    GLuint topSky;
};

