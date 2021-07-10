#pragma once

#define GL_SILENCE_DEPRECATION
#define GLFW_INCLUDE_GLCOREARB
#include <GLFW/glfw3.h>
#include <glm/vec3.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <vector>

class Model3D {
public:
  bool LoadObj(const char* filepath);
  void Transform(const glm::mat4& matrix);
  void VerticesGLBufferData(GLuint vertex_buffer, GLuint attribute_index);
  void TextureGLBufferData(GLuint uv_buffer, GLuint attribute_index);
  void NormalGLBufferData(GLuint normal_buffer, GLuint attribute_index);
  void DrawArrays(GLuint vertex_buffer);

private:
  std::vector<glm::vec3> vertices_;
  std::vector<glm::vec2> texture_; 
  std::vector<glm::vec3> normal_;
};