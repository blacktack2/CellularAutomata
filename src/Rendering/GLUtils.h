#pragma once
#include "glad/glad.h"

void glCheckError_(const char* file, int line);
#define glCheckError() glCheckError_(__FILE__, __LINE__)

GLuint createBuffer(const GLvoid* data, GLsizeiptr size, GLuint binding);
void swapBuffers(GLuint& bufferA, GLuint& bufferB, GLuint bindingA, GLuint bindingB);
void readBuffer(GLuint bufferID, GLvoid* data, GLsizeiptr size);
void writeBuffer(GLuint bufferID, GLvoid* data, GLsizeiptr size);
