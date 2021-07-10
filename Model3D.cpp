#include "Model3D.h"
#include <stdio.h>

bool Model3D::LoadObj(const char* filepath) 
{
  std::vector<glm::vec3> vertex_table;
  std::vector<glm::vec2> texture_table;
  std::vector<glm::vec3> normal_table;
  vertices_.clear();
  texture_.clear();
  normal_.clear();

  FILE* file = fopen(filepath, "r");
  if (!file) {
    fprintf(stderr, "Can't open %s\n", filepath);
    return false;
  }

  char line[1024] = {};
  while (fgets(line, sizeof(line), file)) {
    // Vertex element for the table
    if (strncmp("v ", line, 2) == 0) { 
      float x, y, z;
      int nb_read = sscanf(line, "v %f %f %f", &x, &y, &z);
      
      if (nb_read == 3) {
        vertex_table.push_back(glm::vec3(x, y, z));
      }
    }

    // Texture element for the table
    if (strncmp("vt ", line, 2) == 0) { 
      float x, y;
      int nb_read = sscanf(line, "vt %f %f", &x, &y);
      
      if (nb_read == 2) {
        texture_table.push_back(glm::vec2(x, y));
      }
    }

    // Normal element for the table
    if (strncmp("vn ", line, 2) == 0) { 
      float x, y, z;
      int nb_read = sscanf(line, "vn %f %f %f", &x, &y, &z);
      
      if (nb_read == 3) {
        normal_table.push_back(glm::vec3(x, y, z));
      }
    }

    // Face composed of 3 vertex
    if (strncmp("f ", line, 2) == 0) {
      size_t id_vertex[3];
      size_t id_texture[3];
      size_t id_normal[3];
      int nb_read = sscanf(line, "f %zu/%zu/%zu %zu/%zu/%zu %zu/%zu/%zu", 
          &id_vertex[0], &id_texture[0], &id_normal[0],
          &id_vertex[1], &id_texture[1], &id_normal[1],
          &id_vertex[2], &id_texture[2], &id_normal[2]);

      if (nb_read == 9) {
        // Vertex
        for (size_t i = 0; i < sizeof(id_vertex)/sizeof(id_vertex[0]); i++) {
          id_vertex[i]--; // the first indice is 1

          if (id_vertex[i] >= vertex_table.size()) {
            fprintf(stderr, "Invalid vertex indice %zu at 0x%lx: '%s'\n", id_vertex[i], ftell(file), line);
            fclose(file);
            return false;
          }
          vertices_.push_back(vertex_table[id_vertex[i]]);
        }

        // Texture
        for (size_t i = 0; i < sizeof(id_texture)/sizeof(id_texture[0]); i++) {
          id_texture[i]--; // the first indice is 1

          if (id_texture[i] >= texture_table.size()) {
            fprintf(stderr, "Invalid texture indice %zu at 0x%lx: '%s'\n", id_texture[i], ftell(file), line);
            fclose(file);
            return false;
          }
          texture_.push_back(texture_table[id_texture[i]]);
        }

        // Normal
        for (size_t i = 0; i < sizeof(id_normal)/sizeof(id_normal[0]); i++) {
          id_normal[i]--; // the first indice is 1

          if (id_normal[i] >= normal_table.size()) {
            fprintf(stderr, "Invalid normal indice %zu at 0x%lx: '%s'\n", id_normal[i], ftell(file), line);
            fclose(file);
            return false;
          }
          normal_.push_back(normal_table[id_normal[i]]);
        }
      }
    }
  }

  // TODO: check vertices_.size() == normal_.size() == texture_.size()

  fclose(file);
  return true;
}

void Model3D::Transform(const glm::mat4& matrix)
{
  for (size_t i = 0; i < vertices_.size(); i++) {
    glm::vec4 vector =  matrix * glm::vec4(vertices_[i].x, vertices_[i].y, vertices_[i].z, 1.f);
    vertices_[i].x = vector.x;
    vertices_[i].y = vector.y;
    vertices_[i].z = vector.z;
  }

  for (size_t i = 0; i < normal_.size(); i++) {
    glm::vec4 vector =  matrix * glm::vec4(normal_[i].x, normal_[i].y, normal_[i].z, 1.f);
    normal_[i].x = vector.x;
    normal_[i].y = vector.y;
    normal_[i].z = vector.z;
  }
}

void Model3D::VerticesGLBufferData(GLuint vertex_buffer, GLuint attribute_index)
{
  glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
  glBufferData(GL_ARRAY_BUFFER, vertices_.size() * sizeof(glm::vec3), vertices_.data(), GL_STATIC_DRAW);
  glVertexAttribPointer(attribute_index, 3, GL_FLOAT, GL_FALSE, 0,  nullptr);
  glEnableVertexAttribArray(attribute_index);
}

void Model3D::TextureGLBufferData(GLuint uv_buffer, GLuint attribute_index)
{
  glBindBuffer(GL_ARRAY_BUFFER, uv_buffer);
  glBufferData(GL_ARRAY_BUFFER, texture_.size() * sizeof(glm::vec2), texture_.data(), GL_STATIC_DRAW);
  glVertexAttribPointer(attribute_index, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
  glEnableVertexAttribArray(attribute_index);
}

void Model3D::NormalGLBufferData(GLuint normal_buffer, GLuint attribute_index)
{
  glBindBuffer(GL_ARRAY_BUFFER, normal_buffer);
  glBufferData(GL_ARRAY_BUFFER, normal_.size() * sizeof(glm::vec3), normal_.data(), GL_STATIC_DRAW);
  glVertexAttribPointer(attribute_index, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
  glEnableVertexAttribArray(attribute_index);
}

void Model3D::DrawArrays(GLuint vertex_buffer) 
{
  glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
  glDrawArrays(GL_TRIANGLES, 0, vertices_.size());
}