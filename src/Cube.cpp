#include "Cube.h"
#include "PrimitiveRenderer.h"
#include <glm/gtx/string_cast.hpp>

Cube::Cube(float size, float x, float y, float z, const float* color) {
    // Define vertices for a cube centered at (x, y, z)
    vertices = {
        x - size, y - size, z - size,
        x + size, y - size, z - size,
        x + size, y + size, z - size,
        x - size, y + size, z - size,
        x - size, y - size, z + size,
        x + size, y - size, z + size,
        x + size, y + size, z + size,
        x - size, y + size, z + size
    };

    indices = {
        0, 1, 2, 2, 3, 0, // Front
        4, 5, 6, 6, 7, 4, // Back
        0, 1, 5, 5, 4, 0, // Bottom
        2, 3, 7, 7, 6, 2, // Top
        0, 3, 7, 7, 4, 0, // Left
        1, 2, 6, 6, 5, 1  // Right
    };

    for (int i = 0; i < 8; ++i) {
        colors.push_back(color[0]);
        colors.push_back(color[1]);
        colors.push_back(color[2]);
    }

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    // Vertex positions
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

    // Initialize textures with 0 (no texture)
    textures.fill(0);
}

Cube::~Cube() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void Cube::setTextureForSide(int side, GLuint textureID) {
    if (side >= 0 && side < 6) {
        textures[side] = textureID;
    }
}

void Cube::draw() {
    glBindVertexArray(VAO);

    for (int i = 0; i < 6; ++i) {
        if (textures[i] != 0) {
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, textures[i]);
        }
        else {
            glDisable(GL_TEXTURE_2D);
            glColor3f(colors[0], colors[1], colors[2]);
        }

        // Draw each face (2 triangles per face, 6 vertices total)
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(i * 6 * sizeof(unsigned int)));
    }

    glBindVertexArray(0);
}


void Cube::translate(const glm::vec3& direction) {
    for (size_t i = 0; i < vertices.size(); i += 3) {
        vertices[i] += direction.x;
        vertices[i + 1] += direction.y;
        vertices[i + 2] += direction.z;
    }
}

void Cube::rotate(float angle, const glm::vec3& axis) {
    glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(angle), axis);
    for (size_t i = 0; i < vertices.size(); i += 3) {
        glm::vec4 vertex(vertices[i], vertices[i + 1], vertices[i + 2], 1.0f);
        vertex = rotationMatrix * vertex;
        vertices[i] = vertex.x;
        vertices[i + 1] = vertex.y;
        vertices[i + 2] = vertex.z;
    }
}

void Cube::rotatePoint(float angle, const glm::vec3& axis, const glm::vec3& point) {
    this->translate(-point);

    this->rotate(angle, axis);

    this->translate(point);
}

void Cube::scale(float sx, float sy) {
    glm::vec3 center(0.0f);
    for (size_t i = 0; i < vertices.size(); i += 3) {
        center.x += vertices[i];
        center.y += vertices[i + 1];
        center.z += vertices[i + 2];
    }
    center /= static_cast<float>(vertices.size() / 3);

    for (size_t i = 0; i < vertices.size(); i += 3) {
        vertices[i] = center.x + (vertices[i] - center.x) * sx;
        vertices[i + 1] = center.y + (vertices[i + 1] - center.y) * sy;
    }
}

