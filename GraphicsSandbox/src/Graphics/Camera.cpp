#include "Camera.h"
#include <GL\glew.h>
#include <gtc\matrix_transform.hpp>

#define CALC_CAM_MAT() \
float widthDiv2 = size / 2.0f; \
float heightDiv2 = widthDiv2 / aspectRatio; \
cameraMatrix = glm::ortho(-widthDiv2, widthDiv2, -heightDiv2, heightDiv2, nearPlane, farPlane)

Camera::Camera(float aspectRatio_, float size_, float nearPlane_, float farPlane_, DirectionFacing direction)
  : aspectRatio(aspectRatio_), size(size_), nearPlane(nearPlane_), farPlane(farPlane_), facing(direction)
{
  CALC_CAM_MAT();
}

Camera::~Camera() {}

void Camera::SetSize(const float size_)
{
  size = size_;
}

const float Camera::GetSize() const
{
  return size;
}

void Camera::SetAspect(const float aspect_)
{
  aspectRatio = aspect_;
  CALC_CAM_MAT();
}

const float Camera::GetAspect() const
{
  return aspectRatio;
}

void Camera::SetDirection(const DirectionFacing direction)
{
  facing = direction;
}

const Camera::DirectionFacing Camera::GetDirection() const
{
  return facing;
}

const glm::mat4& Camera::GetMtx() const
{
  return cameraMatrix;
}