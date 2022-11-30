R"(#version 460 core

uniform vec4 viewOffset = vec4(0, 0, 1, 1);

in vec2 position;

out Vertex {
	vec2 texCoord;
} OUT;

void main() {
	gl_Position = vec4(position, 0.0, 1.0);
	vec2 scaledPos = position * viewOffset.zw + viewOffset.xy;
	OUT.texCoord = scaledPos * 0.5 + 0.5;
}
)"