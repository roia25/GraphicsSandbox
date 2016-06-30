#ifndef CAMERA_H
#define CAMERA_H

#include <glm.hpp>

class Camera
{
  public:
    enum struct DirectionFacing
    {
      NegativeZ,
      PositizeZ
    };

    Camera(float aspectRatio_ = 16.0f / 9.0f, float size_ = 20.0f,
      float nearPlane_ = 0.01f, float farPlane_ = 100.0f, DirectionFacing direction = DirectionFacing::NegativeZ);
    ~Camera();
    void SetSize(const float size);
    const float GetSize() const;
    void SetAspect(const float aspect);
    const float GetAspect() const;
    void SetDirection(const DirectionFacing direction);
    const DirectionFacing GetDirection() const;

    const glm::mat4& GetMtx() const;

  private:
    DirectionFacing facing;
    float nearPlane;
    float farPlane;
    float size;
    float aspectRatio;
    glm::mat4 cameraMatrix;

    Camera(const Camera&) = delete;
    Camera(Camera&&) = delete;
    Camera& operator=(const Camera&) = delete;
    Camera& operator=(Camera&&) = delete;
};

#endif
