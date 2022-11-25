R"(#version 460 core

in vec2 position;

out Vertex {
	vec2 texCoord;
} OUT;

void main() {
	gl_Position = vec4(position, 0.0, 1.0);
	OUT.texCoord = position * 0.5 + 0.5;
}
)"