#pragma once

#include "myGlew.h"
#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>

using glm::vec3;
using glm::mat4;

class Camera
{
public:
    Camera();
    ~Camera();

    const mat4& getView() const;
    void update();

    void setPosition(const vec3&);
    void setTarget(const vec3&);
    void setUp(const vec3&);

    vec3& getPosition();
    vec3& getTarget();
    vec3& getUP();

private:
    vec3 eyePosition;
    vec3 targetPosition;
    vec3 up;

    mat4 view;

};

