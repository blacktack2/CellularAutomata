#pragma once
#include "glad/glad.h"

class ComputeShader {
public:
	ComputeShader(const GLchar* const* code, GLsizei count = 1);

	inline GLuint getProgram() { return mProgramID; }

	inline void bind() { glUseProgram(mProgramID); }
	inline void unbind() { glUseProgram(0); }

	void run(GLuint x, GLuint y, GLuint z);
private:
	GLuint mProgramID;

	bool mInitSuccess = false;
};
