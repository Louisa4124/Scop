#include "../include/Buffer.hpp"

// --- VertexBuffer ---

VertexBuffer::VertexBuffer(const void *data, unsigned int size, GLenum usage)
{
    // Generer le VBO et copier les sommets dans le tampon
    glGenBuffers(1, &m_id);
    glBindBuffer(GL_ARRAY_BUFFER, m_id);
    glBufferData(GL_ARRAY_BUFFER, size, data, usage);
}

VertexBuffer::~VertexBuffer()
{
    glDeleteBuffers(1, &m_id);
}

void VertexBuffer::bind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, m_id);
}

void VertexBuffer::unbind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

// --- IndexBuffer (EBO) ---

IndexBuffer::IndexBuffer(const unsigned int *data, unsigned int count, GLenum usage)
    : m_count(count)
{
    // Generer l'EBO et copier les indices dans le tampon
    glGenBuffers(1, &m_id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, usage);
}

IndexBuffer::~IndexBuffer()
{
    glDeleteBuffers(1, &m_id);
}

void IndexBuffer::bind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
}

void IndexBuffer::unbind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

// --- VertexArray (VAO) ---

VertexArray::VertexArray()
{
    glGenVertexArrays(1, &m_id);
}

VertexArray::~VertexArray()
{
    glDeleteVertexArrays(1, &m_id);
}

void VertexArray::bind() const
{
    glBindVertexArray(m_id);
}

void VertexArray::unbind() const
{
    glBindVertexArray(0);
}

void VertexArray::addBuffer(const VertexBuffer &vbo, GLuint layoutLocation, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer)
{
    bind();
    vbo.bind();
    // Initialiser les pointeurs d’attributs de sommets
    glVertexAttribPointer(layoutLocation, size, type, normalized, stride, pointer);
    glEnableVertexAttribArray(layoutLocation);
}