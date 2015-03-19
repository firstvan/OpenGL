#pragma once


class Torus
{
private:
    unsigned int vaoHandle;
    int faces, rings, sides;

    void generateVerts(float *, float *, float *, unsigned int *,
                       float, float);

public:
    Torus(float, float, int, int);

    void render() const;

    int getVertexArrayHandle();
};