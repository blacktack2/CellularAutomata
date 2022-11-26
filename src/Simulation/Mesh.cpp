#include "Mesh.h"

Mesh::Mesh() {
    glGenVertexArrays(1, &mArrayObject);
}

Mesh::~Mesh() {
    glDeleteVertexArrays(1, &mArrayObject);

    delete[] mVertices2;
    delete[] mVertices3;
    delete[] mVertices4;
}

void Mesh::draw() const {
    glBindVertexArray(mArrayObject);
    glDrawArrays(mType, 0, mNumVertices);
    glBindVertexArray(0);
}

Mesh *Mesh::generateScreenQuad() {
    Mesh* mesh = new Mesh();
    mesh->mType = GL_TRIANGLE_STRIP;
    mesh->mNumVertices = 4;

    mesh->mVertices2 = new glm::vec2[4]{
        glm::vec2(-1.0f, -1.0f),
        glm::vec2(1.0f, -1.0f),
        glm::vec2(-1.0f, 1.0f),
        glm::vec2(1.0f, 1.0f)
    };

    mesh->bufferData();
    return mesh;
}

Mesh *Mesh::getScreenQuad() {
    static Mesh* screenQuad = generateScreenQuad();
    return screenQuad;
}

void Mesh::bufferData() {
    glBindVertexArray(mArrayObject);

    if (mVertices2)
        uploadAttribute(VBOIndex::VERTICES2, mNumVertices, sizeof(glm::vec2), 2, mVertices2, "Vertices2");
    if (mVertices3)
        uploadAttribute(VBOIndex::VERTICES3, mNumVertices, sizeof(glm::vec3), 3, mVertices3, "Vertices3");
    if (mVertices4)
        uploadAttribute(VBOIndex::VERTICES4, mNumVertices, sizeof(glm::vec4), 4, mVertices4, "Vertices4");

    glBindVertexArray(0);
}

void Mesh::uploadAttribute(VBOIndex vboIndex, GLuint numElements, GLsizei dataTypeSize, GLint attribSize, const void* data, const char* debugName) {
    GLuint& id = mVBOs[(size_t)vboIndex];
    glGenBuffers(1, &id);
    glBindBuffer(GL_ARRAY_BUFFER, id);

    glBufferData(GL_ARRAY_BUFFER, numElements * dataTypeSize, data, GL_STATIC_DRAW);
    glVertexAttribPointer((GLuint)vboIndex, attribSize, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray((GLuint)vboIndex);

    glObjectLabel(GL_BUFFER, id, -1, debugName);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
