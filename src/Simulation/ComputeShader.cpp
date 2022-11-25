#include "ComputeShader.h"

#include "GLUtils.h"

ComputeShader::ComputeShader(const GLchar* const* code, GLsizei count) {
	GLint success;

	mProgramID = glCreateProgram();

	GLuint shader = glCreateShader(GL_COMPUTE_SHADER);
	glShaderSource(shader, count, code, nullptr);
	glCompileShader(shader);

	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (success != GL_TRUE) {
		GLchar infoLog[512];
		glGetShaderInfoLog(shader, 512, nullptr, infoLog);
		return;
	}
	glAttachShader(mProgramID, shader);

	glDeleteShader(shader);
	glLinkProgram(mProgramID);

	glGetProgramiv(mProgramID, GL_LINK_STATUS, &success);
	if (success != GL_TRUE) {
		GLchar infoLog[512];
		glGetShaderInfoLog(shader, 512, nullptr, infoLog);
		return;
	}

	mInitSuccess = true;
}

void ComputeShader::run(GLuint x, GLuint y, GLuint z) {
	glUseProgram(mProgramID);
	glDispatchCompute(x, y, z);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
	glUseProgram(0);

#ifdef _DEBUG
	glCheckError();
#endif
}
