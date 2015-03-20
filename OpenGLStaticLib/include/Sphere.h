#pragma once
#include "myGlew.h"

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

    int getVertexArrayHandle();
};