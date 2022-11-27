#pragma once
#include <glad/glad.h>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

class Mesh {
public:
    Mesh();
    ~Mesh();

    void draw() const;

    static Mesh* generateScreenQuad();

    static Mesh* getScreenQuad();
private:
    enum class VBOIndex {
        VERTICES2 = 0u,
        VERTICES3,
        VERTICES4,
        MAX
    };

    void bufferData();
    void uploadAttribute(VBOIndex vboIndex, GLuint numElements, GLsizei dataTypeSize, GLint attribSize, const void* data, const char* debugName);

    GLuint mArrayObject = 0;
    GLuint mVBOs[(size_t)VBOIndex::MAX]{};

    GLuint mType = GL_TRIANGLES;
    GLuint mNumVertices = 0;

    glm::vec2* mVertices2 = nullptr;
    glm::vec3* mVertices3 = nullptr;
    glm::vec4* mVertices4 = nullptr;
};