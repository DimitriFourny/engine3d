#version 330 core

in vec3 normal_cameraspace;
in vec3 lightDirection_cameraspace;
in float distance_vertex_light;

out vec3 color;

uniform sampler2D texture_sampler;

void main()
{
  vec3 material_color = vec3(168/255.0, 192/255.0, 41/255.0);
  vec3 ambiant = material_color * vec3(0.3, 0.3, 0.3);

  // vec3 light_color = vec3(103/255.0, 34/255.0, 66/255.0);
  vec3 light_color = vec3(1, 1, 1);
  float light_power = 2000.f;

  // Light angle
  vec3 n = normalize(normal_cameraspace);
  vec3 l = normalize(lightDirection_cameraspace);
  float cosTheta = clamp(dot(n, l), 0, 1);

  color = ambiant + material_color * light_color * light_power * cosTheta / distance_vertex_light;
}