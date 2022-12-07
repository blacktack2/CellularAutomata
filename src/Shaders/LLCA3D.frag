R"(#version 460 core

in Vertex {
	vec2 texCoord;
} IN;

out vec4 fragColour;

void main() {
	fragColour = vec4(1, 0, 0, 1);
}
)"