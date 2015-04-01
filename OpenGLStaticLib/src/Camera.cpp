#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>

Camera::Camera(vec3 eye, float movingSp):
    eyePosition(eye), movingSpace(movingSp), omega(90.0f), fi(0.0f)
{
    front = eyePosition;
    rotate(0.0f, 0.0f);
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
    view = glm::lookAt(eyePosition, eyePosition - front, up);
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
    return eyePosition - front;
}

vec3& Camera::getUP()
{
    return up;
}

void Camera::rotate(float o, float f)
{

    o *= 0.1f;
    f *= 0.1f;
    omega += o;
    fi += f;

    if (fi > 89.0f)
    {
        fi = 89.0f;
    }

    if (fi < -89.0f)
    {
        fi = -89.0f;
    }

    vec3 temp;

    temp.x = glm::cos(glm::radians(omega)) * glm::cos(glm::radians(fi));
    temp.y =  glm::sin(glm::radians(fi));
    temp.z = glm::sin(glm::radians(omega)) * glm::cos(glm::radians(fi));
    front = glm::normalize(temp);
    right = glm::normalize(glm::cross(front, vec3(0.0f, 1.0f, 0.0f)));
    up = glm::normalize(glm::cross(right, front));

    update();
}

void Camera::moving(Dir dir)
{
    vec3 temp;
    switch (dir)
    {
    case Dir::W:
        temp = eyePosition - (front * 0.25f * sprint);

        if (std::abs(temp.x) < movingSpace && std::abs(temp.y) < movingSpace && std::abs(temp.z) < movingSpace)
        {
            eyePosition = temp;
        }

        break;
    case Dir::S:
        temp = eyePosition + (front * 0.25f * sprint);

        if (std::abs(temp.x) < movingSpace && std::abs(temp.y) < movingSpace && std::abs(temp.z) < movingSpace)
        {
            eyePosition = temp;
        }
        break;
    case Dir::A:
        temp = eyePosition + (right * 0.25f * sprint);

        if (std::abs(temp.x) < movingSpace && std::abs(temp.y) < movingSpace && std::abs(temp.z) < movingSpace)
        {
            eyePosition = temp;
        }
        break;
    case Dir::D:
        temp = eyePosition - (right * 0.25f * sprint);

        if (std::abs(temp.x) < movingSpace && std::abs(temp.y) < movingSpace && std::abs(temp.z) < movingSpace)
        {
            eyePosition = temp;
        }
        break;
    case Dir::UP:
        temp = eyePosition + (up * 0.25f);

        if (std::abs(temp.x) < movingSpace && std::abs(temp.y) < movingSpace && std::abs(temp.z) < movingSpace)
        {
            eyePosition = temp;
        }
        break;
    case Dir::DOWN:
        temp = eyePosition - (up * 0.25f);

        if (std::abs(temp.x) < movingSpace && std::abs(temp.y) < movingSpace && std::abs(temp.z) < movingSpace)
        {
            eyePosition = temp;
        }
        break;
    }

    update();
}

void Camera::setSprint(float s)
{
    sprint = s;
}