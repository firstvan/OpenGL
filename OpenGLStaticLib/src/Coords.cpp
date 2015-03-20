#include "Coords.h"

#include <stddef.h>

Coords::Coords(float m)
{
    max = m;

    int position[] =
    {
        0.0, 0.0, 0.0,
        max, 0.0, 0.0,
        0.0, 0.0,0.0,
        0.0, max, 0.0,
        0.0, 0.0,0.0,
        0.0, 0.0, max
    };

    unsigned int handle[1];
    glGenBuffers(1, handle);

    glGenVertexArrays(1, &vaohandle);
    glBindVertexArray(vaohandle);

    glBindBuffer(GL_ARRAY_BUFFER, handle[0]);
    glBufferData(GL_ARRAY_BUFFER, 18 * sizeof(float), position, GL_STATIC_DRAW);
    glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(0);  // Vertex position


    glBindVertexArray(0);

}

void Coords::render()
{
    //glBindVertexArray(vaohandle);
    //glDrawArrays(GL_TRIANGLES, 0, 3);
    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_LINES);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(max, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, max, 0.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, max);
    glEnd();
}

Coords::~Coords()
{
}
