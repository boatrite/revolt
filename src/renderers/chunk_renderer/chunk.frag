#version 330 core
out vec4 FragColor;

in vec3 Color;
in vec3 Normal;

void main() {
  vec3 rgb_normal = Normal * 0.5 + 0.5; // transforms from [-1,1] to [0,1]
  FragColor = vec4(rgb_normal, 1.0);
}
