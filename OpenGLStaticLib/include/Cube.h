#pragma once

#include "myGlew.h"
#include <glm/glm.hpp>

class Cube
{

private:
    unsigned int vaoHandle;
    float vertex[24 * 3];
public:
    Cube(bool, float);

    glm::vec3 getVertex(int i);
    void render();

};

