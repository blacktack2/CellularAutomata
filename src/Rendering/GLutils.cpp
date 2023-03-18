#include "GLUtils.h"

#include <iostream>
#include <string>

void glCheckError_(const char* file, int line) {
	GLenum errorCode;
	while ((errorCode = glGetError()) != GL_NO_ERROR) {
		std::string error;
		switch (errorCode) {
			case GL_INVALID_ENUM                  : error = "INVALID_ENUM"                    ; break;
			case GL_INVALID_VALUE                 : error = "GL_INVALID_VALUE"                ; break;
			case GL_INVALID_OPERATION             : error = "GL_INVALID_OPERATION"            ; break;
			case GL_STACK_OVERFLOW                : error = "GL_STACK_OVERFLOW"               ; break;
			case GL_STACK_UNDERFLOW               : error = "GL_STACK_UNDERFLOW"              ; break;
			case GL_OUT_OF_MEMORY                 : error = "GL_OUT_OF_MEMORY"                ; break;
			case GL_INVALID_FRAMEBUFFER_OPERATION : error = "GL_INVALID_FRAMEBUFFER_OPERATION"; break;
			default                               : error = "__INVALID_ERROR_CODE__"          ; break;
		}
		std::cout << "Error in file '" << file << "' at LINE:" << line << " | " << error << "\n";
	}
}

GLuint createBuffer(const GLvoid* data, GLsizeiptr size, GLuint binding) {
	GLuint id;
	glGenBuffers(1, &id);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, id);
	glBufferData(GL_SHADER_STORAGE_BUFFER, size, data, GL_DYNAMIC_READ);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, binding, id);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
#ifdef _DEBUG
	glCheckError();
#endif
	return id;
}

void swapBuffers(GLuint& bufferA, GLuint& bufferB, GLuint bindingA, GLuint bindingB) {
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, bufferA);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, bindingB, bufferA);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, bufferB);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, bindingA, bufferB);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	std::swap(bufferA, bufferB);
#ifdef _DEBUG
	glCheckError();
#endif
}

void readBuffer(GLuint bufferID, GLvoid* data, GLsizeiptr size) {
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, bufferID);
	glGetBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, size, data);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
#ifdef _DEBUG
	glCheckError();
#endif
}

void writeBuffer(GLuint bufferID, GLvoid* data, GLsizeiptr size) {
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, bufferID);
	glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, size, data);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
#ifdef _DEBUG
	glCheckError();
#endif
}
