#ifndef SPRITE_H
#define SPRITE_H

#include "GraphicsEngine.h"

#include <Core/Component.h>

struct TexelPair
{
  struct
  {
    float x;
    float y;
  } blTexCoord;
  struct
  {
    float x;
    float y;
  } trTexCoord;
};

class Texture
{
public:
  GLuint texID;
  TexelPair *texCoords;
  int frames;
  std::string textureName;
  std::vector<glm::mat4> transformationList;
};


template <typename T>
class GraphicsComponent : public SystemComponent<T, GraphicsEngine>
{
};

class Sprite : public GraphicsComponent<Sprite>
{
  public:
    static std::string name;

    bool visible;
    bool animationActive;
    bool flipX;
    bool flipY;
    int startFrame;
    int currentFrame;
    float animationSpeed;

    Texture *spriteSource;
    Shader *shader;

    Sprite();
    Sprite(Texture *textureSource);
    Sprite(std::string& textureName);
    ~Sprite();

    void SetTexture(std::string& textureName);
    const std::string& GetTexture() const;
    void SetColor(float red, float green, float blue, float alpha);
    const glm::vec4& GetColor() const;

    void SetShader(std::string& shaderName = std::string("src/Shaders/basic"));


  private:
    glm::vec4 color;
    float timeSinceAnimationFrameChange;
    
};

#endif