#version 330 core

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

out vec4 FragColor;

void main() {
	FragColor = vec4(0.0, 1.0, 0.95, 1.0);
}