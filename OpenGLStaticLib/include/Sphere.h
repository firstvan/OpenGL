#pragma once
#include "myGlew.h"
#include <glm\glm.hpp>

class Sphere
{
private:
    unsigned int vaoHandle;
    GLuint nVerts, elements;
    float radius, slices, stacks;

    void generateVerts(float *, float *, float *, GLuint *);

public:
    Sphere(float, int, int);

    void render() const;
    glm::vec3 dir = glm::vec3(1.0f, 0.3f, 0.0f);
    int getVertexArrayHandle();
};