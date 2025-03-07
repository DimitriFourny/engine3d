#version 330 core

layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec2 vertexUV;
layout(location = 2) in vec3 vertexNormal_modelspace;

out vec3 normal_cameraspace;
out vec3 lightDirection_cameraspace;
out float distance_vertex_light;

uniform mat4 M;
uniform mat4 V;
uniform mat4 MVP;

void main()
{
  gl_Position = MVP * vec4(vertexPosition_modelspace, 1);

  // Eye direction in cameraspace
  vec3 vertexPosition_worldspace = (M * vec4(vertexPosition_modelspace, 1)).xyz;
  vec3 vertexPosition_cameraspace = (V * M * vec4(vertexPosition_modelspace, 1)).xyz;
  vec3 eyeDirection_cameraspace = vec3(0, 0, 0) - vertexPosition_cameraspace;

  // The light direction
  vec3 lightPosition_worldspace = vec3(10, 100, 10);
  vec3 lightPosition_cameraspace = (V * vec4(lightPosition_worldspace, 1)).xyz;
  lightDirection_cameraspace = lightPosition_cameraspace + eyeDirection_cameraspace;

  normal_cameraspace = (V * M * vec4(vertexNormal_modelspace, 0)).xyz;

  distance_vertex_light = abs(
      pow(vertexPosition_worldspace.x - lightPosition_worldspace.x, 2) +
      pow(vertexPosition_worldspace.y - lightPosition_worldspace.y, 2) +
      pow(vertexPosition_worldspace.z - lightPosition_worldspace.z, 2));
}