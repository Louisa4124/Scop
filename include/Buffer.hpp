#ifndef BUFFER_HPP
#define BUFFER_HPP

#include <GL/glew.h>

// Vertex Buffer Object (VBO) => Stocke les donnees
//                            => permet d'envoyer autant de donnees que possible a la carte graphique
class VertexBuffer
{
public:
    VertexBuffer(const void *data, unsigned int size, GLenum usage = GL_STATIC_DRAW);
    ~VertexBuffer();

    void bind() const;
    void unbind() const;

private:
    GLuint m_id;
};

// Element Buffer Objects (EBO) => Tampon qui memorise des indices (pour decider quels sommets sont a afficher et dans quel ordre)
class IndexBuffer
{
public:
    IndexBuffer(const unsigned int *data, unsigned int count, GLenum usage = GL_STATIC_DRAW);
    ~IndexBuffer();

    void bind() const;
    void unbind() const;

    unsigned int getCount() const { return m_count; }

private:
    GLuint m_id;
    unsigned int m_count;
};

// Vertex Array Object (VAO) => Decrit les donnees
class VertexArray
{
public:
    VertexArray();
    ~VertexArray();

    void bind() const;
    void unbind() const;

    // Initialiser les pointeurs d’attributs de sommets
    void addBuffer(const VertexBuffer &vbo, GLuint layoutLocation, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer);

private:
    GLuint m_id;
};

#endif // BUFFER_HPP