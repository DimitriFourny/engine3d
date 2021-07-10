#include "Shader.h"
#include <vector>
#include "File.h"

// static
GLuint Shader::Load(const char* vertex_file_path, const char* fragment_file_path)
{
  GLuint vertex_shader_id = Shader::Compile(vertex_file_path, GL_VERTEX_SHADER);
  if (!vertex_shader_id) {
    return 0;
  }

  GLuint fragment_shader_id = Shader::Compile(fragment_file_path, GL_FRAGMENT_SHADER);
  if (!vertex_shader_id) {
    return 0;
  }

  // Link the program
  printf("Linking program\n");
  GLuint program_id = glCreateProgram();
  glAttachShader(program_id, vertex_shader_id);
  glAttachShader(program_id, fragment_shader_id);
  glLinkProgram(program_id);

  // Check the program
  GLint result;
  glGetProgramiv(program_id, GL_LINK_STATUS, &result);
  int info_log_length;
  glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &info_log_length);
  if (info_log_length > 0){
    std::vector<char> ProgramErrorMessage(info_log_length + 1);
    glGetProgramInfoLog(program_id, info_log_length, NULL, &ProgramErrorMessage[0]);
    fprintf(stderr, "%s\n", &ProgramErrorMessage[0]);
    return 0;
  }
  
  glDetachShader(program_id, vertex_shader_id);
  glDetachShader(program_id, fragment_shader_id);
  
  glDeleteShader(vertex_shader_id);
  glDeleteShader(fragment_shader_id);

  return program_id;
}

// static 
GLuint Shader::Compile(const char* filepath, GLenum shader_type) 
{
  printf("Compiling shader: %s\n", filepath);
  std::vector<uint8_t> source = File::LoadInMemory(filepath);
  char* source_ptr = reinterpret_cast<char*>(source.data());
  GLint source_size = source.size();

  GLuint shader_id = glCreateShader(shader_type);
  glShaderSource(shader_id, 1, &source_ptr, &source_size);
  glCompileShader(shader_id);

  // Check if an error happened
  GLint result;
  glGetShaderiv(shader_id, GL_COMPILE_STATUS, &result);
  int info_log_length;
  glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &info_log_length);
  if (info_log_length > 0) {
    std::vector<char> error_message(info_log_length + 1);
    glGetShaderInfoLog(shader_id, info_log_length, NULL, &error_message[0]);
    fprintf(stderr, "%s\n", &error_message[0]);
    return 0;
  }

  return shader_id;
}