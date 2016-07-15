#include "GraphicsEngine.h"
#include <SOIL.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include "Sprite.h"

#include <Core/Component.h>
#include <Core/GameObject.h>

#define INIT_ERROR(MESSAGE)       \
  {                               \
    std::cerr << MESSAGE << "\n"; \
    throw(-1);                    \
  }

#define INIT_ERROR_FUNC(MESSAGE, FUNCTION) \
  {                                        \
    std::cerr << MESSAGE << "\n";          \
    FUNCTION ();                           \
    throw(-1);                             \
  }

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GL_TRUE);
}

GLuint generateMultiSampleTexture(GLuint samples, int screenWidth, int screenHeight)
{
  GLuint texture;
  glGenTextures(1, &texture);

  glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, texture);
  glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, GL_RGB, screenWidth, screenHeight, GL_TRUE);
  glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);

  return texture;
}

static GLuint shaderProgram;
static GLuint vao;
static GLuint vbo;
static GLuint ebo;

static std::vector<GameObject *> ObjectList;

static GLfloat vertices[] = {
  // Positions        // Texture Coords
  0.5f,  0.5f, 0.0f, 1.0f, 1.0f,   // Top Right
  0.5f, -0.5f, 0.0f, 1.0f, 0.0f,   // Bottom Right
  -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,   // Bottom Left
  -0.5f,  0.5f, 0.0f, 0.0f, 1.0f    // Top Left 
};

static GLuint indices[] = {
  0, 1, 3, // First Triangle
  1, 2, 3  // Second Triangle
};

void GraphicsEngine::init()
{
  if (glfwInit() != GL_TRUE)
    INIT_ERROR("Failed to initialize GLFW!")

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

  window = glfwCreateWindow(800, 600, "LearnOpenGL", nullptr, nullptr);
  if (window == nullptr)
    INIT_ERROR_FUNC("Failed to instantiate a window!", glfwTerminate)

  glfwMakeContextCurrent(window);

  glewExperimental = GL_TRUE;
  if (glewInit() != GLEW_OK)
    INIT_ERROR("Failed to initialize GLEW!")

  glfwGetFramebufferSize(window, &width, &height);
  glViewport(0, 0, width, height);

  glEnable(GL_BLEND);
  glEnable(GL_MULTISAMPLE);
  glEnable(GL_DEPTH_TEST);

  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  framebuffer;
  glGenFramebuffers(1, &framebuffer);
  glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

  // Create a multisampled color attachment texture
  GLuint textureColorBufferMultiSampled = generateMultiSampleTexture(16, width, height);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, textureColorBufferMultiSampled, 0);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  glfwSetKeyCallback(window, key_callback);

  // Simply too lazy to write assignment operators, so I'm hacking this together.
  new(&camera) Camera((float)(width) / (float)(height));

  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

  LoadTexture(std::string("assets/sample.jpg"));

  for (int i = 0; i < 5; ++i)
  {
    GameObject *obj = new GameObject;
    ObjectList.push_back(obj);
    Transform *trans = obj->AddComponent(Transform);
    trans->position = glm::vec3(i, i, 0);
    obj->AddComponent(Sprite, std::string("assets/sample.jpg"))->SetShader();
  }

  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &vbo);
  glGenBuffers(1, &ebo);

  glBindVertexArray(vao);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  // Position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
  glEnableVertexAttribArray(0);
  // TexCoord attribute
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
  glEnableVertexAttribArray(2);

  glBindVertexArray(0);
}

void GraphicsEngine::update(float dt)
{
  // 1. Bind the multisampled frame buffer, then draw into it with render
  glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

  // Then render ---------------------------------------------------------
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  for (int i = 0; i < ObjectList.size(); ++i)
  {
    GameObject *obj = ObjectList[i];
    Sprite *sprite = obj->GetComponent(Sprite);
    Shader *shader = sprite->shader;
    shader->Use();

    // Create the transformation matrix
    glm::mat4 view;
    glm::mat4 projection;
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
    projection = camera.GetMtx();

    // Get their uniform location
    GLint modelLoc = glGetUniformLocation(shader->program, "model");
    GLint viewLoc = glGetUniformLocation(shader->program, "view");
    GLint projLoc = glGetUniformLocation(shader->program, "projection");
    // Pass the matrices to the shader
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    // Note: currently we set the projection matrix each frame, but since the
    // projection matrix rarely changes it's probably a good idea to
    // just set it once.
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    glm::mat4 model;
    //model = glm::scale(model, glm::vec3(obj->transform->scale, 1.0f));
    //model = glm::rotate(model, obj->transform->rotation, glm::vec3(0.0f, 0.0f, -1.0f));
    model = glm::translate(model, obj->transform->position);

    // Get the matrix's uniform location and set the matrix
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    // Draw the thingy!
    glBindVertexArray(vao);
    glBindTexture(GL_TEXTURE_2D, sprite->spriteSource->texID);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    // Done with render -----------------------------------------------------------------
  }
  // 2. Now blit multisampled buffer(s) to default framebuffers
  glBindFramebuffer(GL_READ_FRAMEBUFFER, framebuffer);
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
  glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
}

void GraphicsEngine::shutdown()
{
  glDeleteVertexArrays(1, &vao);
  glDeleteBuffers(1, &vbo);
  glDeleteBuffers(1, &ebo);
  glfwTerminate();
}

GraphicsEngine::GraphicsEngine() 
{
  init();
}

GraphicsEngine::~GraphicsEngine()
{
  shutdown();
}

Texture *GraphicsEngine::LoadTexture(std::string& fileName)
{
  auto it = textureMap.find(fileName);
  if (it != textureMap.end())
    return it->second;
  else
  {
    Texture *tex = new Texture;
    tex->texID = 0;
    tex->frames = 1;
    tex->textureName = fileName;

    // Make a texture coordinate pair for this texture
    tex->texCoords = static_cast<TexelPair *>(malloc(sizeof(TexelPair) * 1));
    // Set the bottom left texture coordinate
    tex->texCoords[0].blTexCoord.x = 0.0f;
    tex->texCoords[0].blTexCoord.y = 0.0f;
    // Set the top right texture coordinate
    tex->texCoords[0].trTexCoord.x = 1.0f;
    tex->texCoords[0].trTexCoord.y = 1.0f;

    int iwidth, iheight;
    unsigned char *image = SOIL_load_image(fileName.c_str(), &iwidth, &iheight, 0, SOIL_LOAD_RGB);
    glGenTextures(1, &tex->texID);
    glBindTexture(GL_TEXTURE_2D, tex->texID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, iwidth, iheight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);

    SOIL_free_image_data(image);
    glBindTexture(GL_TEXTURE_2D, 0);

    textureMap[fileName] = tex;
    return tex;
  }
}

void GraphicsEngine::RegisterComponent(std::string& name, BaseComponent *comp)
{
  auto& compVec = componentMap[name];
  compVec.push_back(comp);
  // In future return a component handle here
}

Shader *GraphicsEngine::LoadShader(std::string& name)
{
  Shader *shader = new Shader((name + ".vert").c_str(), (name + ".frag").c_str());
  shaderMap[name] = shader;
  return shader;
}