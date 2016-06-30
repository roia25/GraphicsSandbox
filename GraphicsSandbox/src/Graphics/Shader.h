#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <iostream>
#include <GL/glew.h>

class Shader
{
  public:
    // OpenGL program ID (the shader program)
    GLuint program;
    Shader(const char *vertexPath = (const char *)("src/Shaders/basic.vert"), 
           const char *fragmentPath = (const char *)("src/Shaders/basic.frag"));
    void Use() const;
};

#endif
