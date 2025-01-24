#ifndef PRIMITIVE_RENDERER_H
#define PRIMITIVE_RENDERER_H

#include <GL/freeglut.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>


class PrimitiveRenderer {
public:

    static void drawPoints(float* vertices, float* colors, int count);

    static void drawLines(float* vertices, float* colors, int count);

    static void drawTriangles(float* vertices, float* colors, int count);

    static void drawIndexedTriangles(const float* vertices, const float* colors, const unsigned int* indices, int indexCount);

    static void drawCube(float size, float x, float y, float z, float* color);

    static void configureMaterial(const glm::vec4& ambient, const glm::vec4& diffuse, const glm::vec4& specular, float shininess);
};

#endif // PRIMITIVE_RENDERER_H
