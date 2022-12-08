R"(#version 460 core

uniform float invNumGenerations = 1;

uniform vec3 liveCol;
uniform vec3 deadCol;

in Vertex {
	uint value;
} IN;

out vec4 fragColour;

void main() {
	fragColour = vec4(mix(deadCol, liveCol, IN.value * invNumGenerations), 1.0);
}
)"