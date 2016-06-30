#include "Shader.h"
#include <assert.h>

static std::string get_file_contents(const char *filename)
{
  std::FILE *fp = std::fopen(filename, "rb");
  if (fp)
  {
    std::string contents;
    std::fseek(fp, 0, SEEK_END);
    contents.resize(std::ftell(fp));
    std::rewind(fp);
    std::fread(&contents[0], 1, contents.size(), fp);
    std::fclose(fp);
    return(contents);
  }
  return std::string("");
}

Shader::Shader(const char *vertexPath, const char *fragmentPath)
{
  // 1. Retrieve the vertex/fragment source code from filePath
  std::string vertexCodeSTD = get_file_contents(vertexPath);
  std::string fragmentCodeSTD = get_file_contents(fragmentPath);
  const GLchar *vertexCode = vertexCodeSTD.c_str();
  const GLchar *fragmentCode = fragmentCodeSTD.c_str();

  // 2. Compile shaders
  GLuint vertex, fragment;
  GLint success;
  GLchar infoLog[512];

  // Vertex Shader
  vertex = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex, 1, &vertexCode, NULL);
  glCompileShader(vertex);
  // Print shader compilation errors if any
  glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    glGetShaderInfoLog(vertex, 512, NULL, infoLog);
    std::cerr << "Error: Vertex shader compilation failed!\n" << infoLog << "\n";
  }

  // Fragment Shader
  fragment = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment, 1, &fragmentCode, NULL);
  glCompileShader(fragment);
  // Print shader compilation errors if any
  glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    glGetShaderInfoLog(fragment, 512, NULL, infoLog);
    std::cerr << "Error: Fragment shader compilation failed!\n" << infoLog << "\n";
  }

  // Shader Program
  program = glCreateProgram();
  glAttachShader(program, vertex);
  glAttachShader(program, fragment);
  glLinkProgram(program);
  // Print linking errors if any
  glGetProgramiv(program, GL_LINK_STATUS, &success);
  if (!success)
  {
    glGetProgramInfoLog(program, 512, NULL, infoLog);
    std::cerr << "Error: Shader program linking failed!\n" << infoLog << "\n";
  }

  glDeleteShader(vertex);
  glDeleteShader(fragment);
}

void Shader::Use() const
{
  GLuint shader_not_initialized = (GLuint)(-1);
  assert(program != shader_not_initialized);
  glUseProgram(program);
}