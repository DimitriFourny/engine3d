#pragma once

#define GL_SILENCE_DEPRECATION
#define GLFW_INCLUDE_GLCOREARB
#include <GLFW/glfw3.h>

class Shader {
public:
  static GLuint Load(const char* vertex_file_path, const char* fragment_file_path);

private:
  static GLuint Compile(const char* filepath, GLenum shader_type);
};