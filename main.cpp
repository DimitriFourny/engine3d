#define GL_SILENCE_DEPRECATION
#define GLFW_INCLUDE_GLCOREARB
#include <GLFW/glfw3.h>
#include <glm/vec3.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include "File.h"
#include "Shader.h"
#include "Model3D.h"

#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 768
#define countof(array) (sizeof(array)/sizeof(array[0]))

glm::vec3 g_camera_pos;
bool move_front;
bool move_back;
bool move_left;
bool move_right;
bool move_up;
bool move_down;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, GLFW_TRUE);
  }

  if (key == GLFW_KEY_W && action != GLFW_REPEAT ) {
    move_front = (action == GLFW_PRESS);
  }
  if (key == GLFW_KEY_S && action != GLFW_REPEAT ) {
    move_back = (action == GLFW_PRESS);
  }
  if (key == GLFW_KEY_A && action != GLFW_REPEAT ) {
    move_left = (action == GLFW_PRESS);
  }
  if (key == GLFW_KEY_D && action != GLFW_REPEAT ) {
    move_right = (action == GLFW_PRESS);
  }
  if (key == GLFW_KEY_SPACE && action != GLFW_REPEAT ) {
    move_up = (action == GLFW_PRESS);
  }
  if (key == GLFW_KEY_LEFT_SHIFT && action != GLFW_REPEAT ) {
    move_down = (action == GLFW_PRESS);
  }
}

double cursor_xpos = 0;
double cursor_ypos = 0;
bool cursor_updated = false;
void cursor_callback(GLFWwindow* window, double xpos, double ypos) 
{
  cursor_xpos = xpos;
  cursor_ypos = ypos;
  cursor_updated = true;
  glfwSetCursorPos(window, WINDOW_WIDTH/2.f, WINDOW_HEIGHT/2.f);
}

int main()
{
  Model3D floor;
  if (!floor.LoadObj("models/floor.obj")) {
    return 1;
  }
  floor.Transform(glm::translate(glm::mat4(1.f), glm::vec3(0.f, -0.5f ,0.f)));

  Model3D model_car;
  if (!model_car.LoadObj("models/car.obj")) {
    return 1;
  }

  Model3D cars[10];
  for (size_t i = 0; i < countof(cars); i++) {
    cars[i] = model_car;
    glm::mat4 translation_matrix = glm::translate(glm::mat4(1.f), glm::vec3((i+1) * 2.5f, 0.f ,0.f));
    cars[i].Transform(translation_matrix);
  }

  if (!glfwInit()) {
      return 1;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

  GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "3D Engine", NULL, NULL);
  if (!window) {
      glfwTerminate();
      return 1;
  }
  glfwMakeContextCurrent(window);
  glfwSetKeyCallback(window, key_callback);
  glfwSetCursorPosCallback(window, cursor_callback);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  GLuint vertex_array_id;
  glGenVertexArrays(1, &vertex_array_id);
  glBindVertexArray(vertex_array_id);

  // Vertex buffer
  GLuint vertex_buffer;
  glGenBuffers(1, &vertex_buffer);

  // Texture buffer
  GLuint uv_buffer;
  glGenBuffers(1, &uv_buffer);

  // Normal buffer
  GLuint normal_buffer;
  glGenBuffers(1, &normal_buffer);

  // Compile the shaders
  GLuint program_id = Shader::Load("shaders/vertex.glsl", "shaders/fragment.glsl");
  if (!program_id) {
    return 1;
  }
  GLuint m_id = glGetUniformLocation(program_id, "M");
  GLuint v_id = glGetUniformLocation(program_id, "V");
  GLuint mvp_id = glGetUniformLocation(program_id, "MVP");

  GLuint floor_program_id = Shader::Load("shaders/vertex_floor.glsl", "shaders/fragment_floor.glsl");
  if (!floor_program_id) {
    return 1;
  }
  GLuint floor_m_id = glGetUniformLocation(floor_program_id, "M");
  GLuint floor_v_id = glGetUniformLocation(floor_program_id, "V");
  GLuint floor_mvp_id = glGetUniformLocation(floor_program_id, "MVP");

  // Load the BMP
  const char texture_filepath[] = "textures/wood.bmp";
  std::vector<uint8_t> buffer = File::LoadInMemory(texture_filepath);
  if (buffer.size() < 54) {
    printf("Can't load %s\n", texture_filepath);
    return 1;
  }

  uint8_t* buffer_data = buffer.data();
  uint32_t data_pos    = *reinterpret_cast<uint32_t*>(&buffer_data[0x0A]);
  uint32_t width       = *reinterpret_cast<uint32_t*>(&buffer_data[0x12]);
  uint32_t height      = *reinterpret_cast<uint32_t*>(&buffer_data[0x16]);
  uint32_t image_size  = *reinterpret_cast<uint32_t*>(&buffer_data[0x22]);
  if (!data_pos) {
    data_pos = 54;
  }
  if (!image_size) {
    image_size = width * height * 3;
  }
  if (data_pos >= buffer.size()) {
    return 1;
  }

  GLuint texture_id;
  glGenTextures(1, &texture_id);
  glBindTexture(GL_TEXTURE_2D, texture_id);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, &buffer_data[data_pos]);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glGenerateMipmap(GL_TEXTURE_2D);

  // Prepare the movement
  g_camera_pos = glm::vec3(0, 3, -6);

  move_front = false;
  move_back = false;
  move_left = false;
  move_right = false;
  double last_time = glfwGetTime();
  double horizontal_angle = 0;
  double vertical_angle = 0;

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glEnable(GL_CULL_FACE);

  while (!glfwWindowShouldClose(window)) {
    glClearColor(0x14/255.f, 0x2b/255.f, 0x43/255.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // FPS
    double current_time = glfwGetTime();
    double delta_time = current_time - last_time;
    last_time = current_time;

    // Camera
    if (cursor_updated) {
      horizontal_angle += 0.1f * delta_time * (WINDOW_WIDTH/2.f - cursor_xpos);
      vertical_angle += 0.1f * delta_time * (WINDOW_HEIGHT/2.f - cursor_ypos);
      cursor_updated = false;
    }

    glm::vec3 direction(
        cos(vertical_angle) * sin(horizontal_angle), sin(vertical_angle), cos(vertical_angle) * cos(horizontal_angle));
    glm::vec3 right = glm::vec3(
        sin(horizontal_angle - 3.14f/2.0f), 0, cos(horizontal_angle - 3.14f/2.0f));
    glm::vec3 up = glm::cross(right, direction);

    if (move_front) {
      g_camera_pos += direction * 10.f * (float)delta_time;
    }
    if (move_back) {
      g_camera_pos -= direction * 10.f * (float)delta_time;
    }
    if (move_left) {
      g_camera_pos -= right * 10.f * (float)delta_time;
    }
    if (move_right) {
      g_camera_pos += right * 10.f * (float)delta_time;
    }
    if (move_up) {
      g_camera_pos += up * 10.f * (float)delta_time;
    }
    if (move_down) {
      g_camera_pos -= up * 10.f * (float)delta_time;
    }

    // Rotation
    // float rotation = 1.f * delta_time;
    // glm::mat4 rotation_matrix = glm::rotate(glm::mat4(1.0f), rotation, glm::vec3(0.f, 0.25f, 0.f));
    // floor.Transform(rotation_matrix);

    // MVP
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), WINDOW_WIDTH/(float)WINDOW_HEIGHT, 0.1f, 100.0f);
    glm::mat4 view = glm::lookAt(g_camera_pos, g_camera_pos + direction, up);
    glm::mat4 model_matrix = glm::mat4(1.0f);
    glm::mat4 mvp = projection * view * model_matrix;

    glUseProgram(floor_program_id);
    glUniformMatrix4fv(floor_m_id, 1, GL_FALSE, &model_matrix[0][0]);
    glUniformMatrix4fv(floor_v_id, 1, GL_FALSE, &view[0][0]);
    glUniformMatrix4fv(floor_mvp_id, 1, GL_FALSE, &mvp[0][0]);

    // Floor
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    floor.VerticesGLBufferData(vertex_buffer, 0);
    floor.TextureGLBufferData(uv_buffer, 1);
    floor.NormalGLBufferData(normal_buffer, 2);
    floor.DrawArrays(vertex_buffer);
    // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    for (GLuint i = 0; i < 3; i++) {
      glDisableVertexAttribArray(i);
    }

    glUseProgram(program_id);
    glUniformMatrix4fv(m_id, 1, GL_FALSE, &model_matrix[0][0]);
    glUniformMatrix4fv(v_id, 1, GL_FALSE, &view[0][0]);
    glUniformMatrix4fv(mvp_id, 1, GL_FALSE, &mvp[0][0]);

    // Cars
    cars[0].TextureGLBufferData(uv_buffer, 1);
    for (size_t i = 0; i < countof(cars); i++) {
      cars[i].VerticesGLBufferData(vertex_buffer, 0);
      cars[i].NormalGLBufferData(normal_buffer, 2);
      cars[i].DrawArrays(vertex_buffer);
    }

    for (GLuint i = 0; i < 3; i++) {
      glDisableVertexAttribArray(i);
    }

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}