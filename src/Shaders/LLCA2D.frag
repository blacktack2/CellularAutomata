R"(#version 460 core

const uint MAX_SIZE = 1000;
const uint MAX_CELLS = MAX_SIZE * MAX_SIZE;

layout(std430, binding = 2) buffer CellOutBuffer {
	uint cells[MAX_CELLS];
};

uniform vec4 viewOffset = vec4(0, 0, 1, 1);

uniform uvec2 simulationBounds = uvec2(100);
uniform vec2 imageBounds = vec2(100.0);
uniform float invNumGenerations = 1;

uniform vec3 liveCol;
uniform vec3 deadCol;

uniform vec4 borderCol;
uniform vec4 gridCol;

in Vertex {
	vec2 texCoord;
} IN;

out vec4 fragColour;

void main() {
	fragColour = vec4(0.0);
	vec2 texCoord = fract(IN.texCoord);
	if (borderCol.w > 0.0 && (any(lessThanEqual(ivec2(texCoord * imageBounds / viewOffset.zw), ivec2(1))))) {
		fragColour = borderCol;
	} else if (gridCol.w > 0.0 && any(lessThanEqual(ivec2(fract(texCoord * simulationBounds) * imageBounds / viewOffset.zw), simulationBounds))) {
		fragColour = gridCol;
	}
	uvec2 pos = uvec2(texCoord * simulationBounds);
	fragColour = vec4(mix(mix(deadCol, liveCol, cells[pos.x + pos.y * MAX_SIZE] * invNumGenerations), fragColour.rgb, fragColour.a), 1.0);
}
)"