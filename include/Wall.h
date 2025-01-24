#ifndef WALL_H
#define WALL_H

#include "DrawableObject.h"
#include "TransformableObject.h"
#include <GL/gl.h>
#include <glm/glm.hpp>

class Wall : public DrawableObject, public TransformableObject {
public:
    Wall(float x, float y, float z, float width, float height, GLuint texture);
    void draw() override;
    void translate(const glm::vec3& direction) override;
    void rotate(float angle, const glm::vec3& axis) override;
    void rotatePoint(float angle, const glm::vec3& axis, const glm::vec3& point) override;
    void rotateAround(float angle, const glm::vec3& axis);
    void scale(float sx, float sy) override;

private:
    float vertices[12];
    GLuint textureID;
    glm::vec3 position;
    glm::vec2 size;
};

#endif // WALL_H