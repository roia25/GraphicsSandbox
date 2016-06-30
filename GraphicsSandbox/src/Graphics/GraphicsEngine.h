#ifndef GRAPHICSENGINE_H
#define GRAPHICSENGINE_H

#include <unordered_map>
#include <vector>

#include <GL\glew.h>
#include <GLFW\glfw3.h>

#include "Shader.h"
#include "Camera.h"

#include "../Core/Component.h"

#include <assert.h>

struct TexelPair;
class Texture;

template <typename T>
class Singleton
{
  private:
    static T *m_Singleton;

  public:
    Singleton()
    {
      assert(!m_Singleton);
      static long long offset = (long long)(T *)(1LL) - (long long)(Singleton<T> *)(T *)(1LL);
      m_Singleton = (T *)((long long)this + offset);
    }

    ~Singleton()
    {
      assert(m_Singleton);
      m_Singleton = 0;
    }

    static T& GetSingleton()
    {
      assert(m_Singleton);
      return *m_Singleton;
    }

    static T *GetSingletonPtr() { return m_Singleton; }
};

template <typename T> T *Singleton<T>::m_Singleton = nullptr;

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

    //static GraphicsEngine *GetSingletonPtr();
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