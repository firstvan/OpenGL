#include "Camera.h"


Camera::Camera()
{
}


Camera::~Camera()
{
}

const mat4& Camera::getView() const
{
    return view;
}

void Camera::update()
{
    view = glm::lookAt(eyePosition, targetPosition, up);
}

void Camera::setPosition(const vec3& p)
{
    eyePosition = p;
}

void Camera::setTarget(const vec3& p)
{
    targetPosition = p;
}

void Camera::setUp(const vec3& p)
{
    up = p;
}

vec3& Camera::getPosition()
{
    return eyePosition;
}

vec3& Camera::getTarget()
{
    return targetPosition;
}

vec3& Camera::getUP()
{
    return up;
}