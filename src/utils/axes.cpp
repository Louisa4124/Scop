#include "../include/axes.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

namespace utils {

static const char *vertexShader = R"(
    #version 330 core
    layout (location = 0) in vec3 position;
    layout (location = 1) in vec3 color;
    out vec3 vertexColor;
    uniform mat4 projection;
    uniform mat4 view;
    void main() {
        gl_Position = projection * view * vec4(position, 1.0);
        vertexColor = color;
    }
)";

static const char *fragmentShader = R"(
    #version 330 core
    in vec3 vertexColor;
    out vec4 FragColor;
    void main() {
        FragColor = vec4(vertexColor, 1.0);
    }
)";

Axes::Axes() {
    // Compile shaders
    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vertexShader, nullptr);
    glCompileShader(vs);

    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fragmentShader, nullptr);
    glCompileShader(fs);

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vs);
    glAttachShader(shaderProgram, fs);
    glLinkProgram(shaderProgram);
    glDeleteShader(vs);
    glDeleteShader(fs);

    // Setup vertex data (X, Y, Z axes)
    float vertices[] = {
        // X axis (red)
        0.0f, 0.0f, 0.0f,   1.0f, 0.0f, 0.0f,
        2.0f, 0.0f, 0.0f,   1.0f, 0.0f, 0.0f,
        // Y axis (green)
        0.0f, 0.0f, 0.0f,   0.0f, 1.0f, 0.0f,
        0.0f, 2.0f, 0.0f,   0.0f, 1.0f, 0.0f,
        // Z axis (blue)
        0.0f, 0.0f, 0.0f,   0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 2.0f,   0.0f, 0.0f, 1.0f,
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

Axes::~Axes() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
}

void Axes::render(const glm::mat4 &projection, const glm::mat4 &view) {
    glUseProgram(shaderProgram);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glBindVertexArray(VAO);
    glDrawArrays(GL_LINES, 0, 6);
}

}