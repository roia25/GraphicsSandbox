#include "Sprite.h"

std::string Sprite::name = "Sprite";

Sprite::Sprite() : visible(true), animationActive(true), flipX(false), flipY(false),
  startFrame(0), currentFrame(0), animationSpeed(1), color(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)),
  spriteSource(nullptr)
{
  shader = gGraphics->LoadShader();
}

Sprite::Sprite(Texture *textureSource) : visible(true), animationActive(true), flipX(false), flipY(false),
  startFrame(0), currentFrame(0), animationSpeed(1), color(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)),
  spriteSource(textureSource)
{
}

Sprite::Sprite(std::string& textureName) : visible(true), animationActive(true), flipX(false), flipY(false),
  startFrame(0), currentFrame(0), animationSpeed(1), color(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f))
{
  SetTexture(textureName);
}

Sprite::~Sprite()
{
}

void Sprite::SetTexture(std::string& textureName)
{
  auto it = gGraphics->textureMap.find(textureName);

  if (it == gGraphics->textureMap.end())
    spriteSource = nullptr;
  else
    spriteSource = it->second;
}

const std::string& Sprite::GetTexture() const
{
  return spriteSource->textureName;
}

void Sprite::SetColor(float red, float green, float blue, float alpha)
{
  color = glm::vec4(red, green, blue, alpha);
}

const glm::vec4& Sprite::GetColor() const
{
  return color;
}

void Sprite::SetShader(std::string& shaderName)
{
  auto it = gGraphics->shaderMap.find(shaderName);

  if (it == gGraphics->shaderMap.end())
    shader = gGraphics->LoadShader(shaderName);
  else
    shader = it->second;
}