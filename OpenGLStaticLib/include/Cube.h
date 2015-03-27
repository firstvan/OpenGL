#pragma once

#include "myGlew.h"

class Cube
{

private:
    unsigned int vaoHandle;

public:
    Cube(bool, float);


    void render();

};

