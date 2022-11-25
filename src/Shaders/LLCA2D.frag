R"(#version 460 core

const uint MAX_SIZE = 2000;
const uint MAX_CELLS = MAX_SIZE * MAX_SIZE;

layout(std430, binding = 2) buffer CellOutBuffer {
	uint cells[MAX_CELLS];
};

uniform uvec2 simulationBounds = uvec2(100, 100);
uniform float invNumGenerations = 1;

uniform vec3 liveCol;
uniform vec3 deadCol;

in Vertex {
	vec2 texCoord;
} IN;

out vec4 fragColor;

void main() {
	uvec2 pos = uvec2(IN.texCoord * simulationBounds);
	fragColor = vec4(mix(deadCol, liveCol, cells[pos.x + pos.y * MAX_SIZE] * invNumGenerations), 1.0);
}
)"