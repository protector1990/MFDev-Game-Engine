#version 330

in vec4 position;
in vec4 color;

out vec4 colorV;

void main() {
	colorV = color * position;
	gl_Position = position;
}