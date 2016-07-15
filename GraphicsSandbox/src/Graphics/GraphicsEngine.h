#ifndef GRAPHICSENGINE_H
#define GRAPHICSENGINE_H

#include <unordered_map>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <Utils/Singleton.h>

#include "Shader.h"
#include "Camera.h"

struct TexelPair;
class Texture;
class BaseComponent;

class GraphicsEngine : public Singleton<GraphicsEngine>
{
  public:
    Camera camera;
    GLuint framebuffer;
    GLFWwindow *window;
    int width;
    int height;

    std::unordered_map<std::string, Texture *> textureMap;
    std::unordered_map<std::string, Shader *> shaderMap;

    GraphicsEngine();
    ~GraphicsEngine();

    void init();
    void update(float dt);
    void shutdown();

    Texture *LoadTexture(std::string& fileName);
    Shader *LoadShader(std::string& name = std::string("src/Shaders/basic"));
    void RegisterComponent(std::string& name, BaseComponent *comp);

  private:
    std::unordered_map<std::string, std::vector<BaseComponent *>> componentMap;
};

#define gGraphics GraphicsEngine::GetSingletonPtr()

#endif