#pragma once
#include "myGlew.h"

class Coords
{
public:
    Coords(float);
    ~Coords();
    void render();
private:
    GLuint vaohandle;
    float max;
};

