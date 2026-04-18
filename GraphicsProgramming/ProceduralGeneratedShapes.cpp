#include "ProceduralGeneratedShapes.h"
#define M_PI 3.14

ProceduralGeneratedShapes::ProceduralGeneratedShapes()
{
}

void ProceduralGeneratedShapes::renderSphere()
{
    float radius = 1.0f;
    int segments = 30;

    for (int lat = 0; lat < segments; lat++)
    {
        float delta = lat * (M_PI / segments);
        float deltaNext = (lat + 1) * (M_PI / segments);

        for (int lon = 0; lon < segments; lon++)
        {
            float theta = lon * (2 * M_PI / segments);
            float thetaNext = (lon + 1) * (2 * M_PI / segments);

            float x1 = radius * cos(theta) * sin(delta);
            float y1 = radius * cos(delta);
            float z1 = radius * sin(theta) * sin(delta);

            float x2 = radius * cos(theta) * sin(deltaNext);
            float y2 = radius * cos(deltaNext);
            float z2 = radius * sin(theta) * sin(deltaNext);

            float x3 = radius * cos(thetaNext) * sin(deltaNext);
            float y3 = radius * cos(deltaNext);
            float z3 = radius * sin(thetaNext) * sin(deltaNext);

            float x4 = radius * cos(thetaNext) * sin(delta);
            float y4 = radius * cos(delta);
            float z4 = radius * sin(thetaNext) * sin(delta);

            glBegin(GL_QUADS);

            glNormal3f(x1, y1, z1);
            glTexCoord2f((float)lon / segments, (float)lat / segments);
            glVertex3f(x1, y1, z1);

            glNormal3f(x2, y2, z2);
            glTexCoord2f((float)lon / segments, (float)(lat + 1) / segments);
            glVertex3f(x2, y2, z2);

            glNormal3f(x3, y3, z3);
            glTexCoord2f((float)(lon + 1) / segments, (float)(lat + 1) / segments);
            glVertex3f(x3, y3, z3);

            glNormal3f(x4, y4, z4);
            glTexCoord2f((float)(lon + 1) / segments, (float)lat / segments);
            glVertex3f(x4, y4, z4);

            glEnd();
        }
    }
}


