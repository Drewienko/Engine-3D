#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>


void loadOpenGLFunctions();

class Shader {
public:
    GLuint ID; // Program ID

    // Constructor: Compiles and links the shader program
    Shader();
    Shader(const char* vertexCode, const char* fragmentCode);
   
    // Activate the shader program
    void use();

    // Utility functions to set uniforms
    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
    void setMat4(const std::string& name, const float* value) const;
    void setVec3(const std::string& name, const glm::vec3& value) const;

private:
    // Helper function to compile individual shaders
    GLuint compileShader(GLenum type, const char* code);
};

#endif
