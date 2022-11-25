#pragma once
#include "glad/glad.h"

class Shader {
public:
	Shader(const GLchar* const* vert, const GLchar* const* frag, GLsizei vertCount = 1, GLsizei fragCount = 1);

	inline GLuint getProgram() { return mProgramID; }

	inline void bind() { glUseProgram(mProgramID); }
	inline void unbind() { glUseProgram(0); }
private:
	GLuint mProgramID;

	bool mInitSuccess = false;
};