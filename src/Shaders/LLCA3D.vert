R"(#version 460 core

const uint MAX_SIZE = 100;
const uint MAX_CELLS = MAX_SIZE * MAX_SIZE * MAX_SIZE;

layout(std430, binding = 2) buffer CellOutBuffer {
	uint cells[MAX_CELLS];
};

uniform mat4 modelMatrix;
uniform mat4 viewProjMatrix;

uniform uvec3 simulationBounds = uvec3(100);

out Vertex {
	uint value;
} OUT;

void main() {
	uvec3 c = uvec3(
		mod(gl_instanceID, simulationBounds.x),
		mod(gl_instanceID / simulationBounds.y, simulationBounds.x),
		gl_instanceID / simulationBounds.z
	);
	OUT.value = cells[c.x + c.y * MAX_SIZE + c.z * MAX_SIZE * MAX_SIZE];
	gl_Position = (OUT.value == 0) ?
		vec4(vec3(0.0), 1.0) :
		viewProjMatrix * modelMatrix * vec4(vec3(c) - vec3(float(MAX_SIZE) * 0.5), 1.0);
}
)"