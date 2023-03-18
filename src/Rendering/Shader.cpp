#include "Shader.h"

Shader::Shader(const GLchar* const* vert, const GLchar* const* frag, GLsizei vertCount, GLsizei fragCount) {
	GLint success;

	mProgramID = glCreateProgram();

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(vertexShader, vertCount, vert, nullptr);
	glShaderSource(fragmentShader, fragCount, frag, nullptr);

	glCompileShader(vertexShader);
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (success != GL_TRUE) {
		GLchar infoLog[512];
		glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
		return;
	}
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (success != GL_TRUE) {
		GLchar infoLog[512];
		glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
		return;
	}

	glAttachShader(mProgramID, vertexShader);
	glAttachShader(mProgramID, fragmentShader);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	glLinkProgram(mProgramID);
	glGetProgramiv(mProgramID, GL_LINK_STATUS, &success);
	if (success != GL_TRUE) {
		GLchar infoLog[512];
		glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
		return;
	}

	mInitSuccess = true;
}
