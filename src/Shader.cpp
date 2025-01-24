#include "Shader.h"


PFNGLCREATESHADERPROC glCreateShader = nullptr;
PFNGLSHADERSOURCEPROC glShaderSource = nullptr;
PFNGLCOMPILESHADERPROC glCompileShader = nullptr;
PFNGLCREATEPROGRAMPROC glCreateProgram = nullptr;
PFNGLATTACHSHADERPROC glAttachShader = nullptr;
PFNGLLINKPROGRAMPROC glLinkProgram = nullptr;
PFNGLUSEPROGRAMPROC glUseProgram = nullptr;
PFNGLGETPROGRAMIVPROC glGetProgramiv = nullptr;
PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog = nullptr;
PFNGLGETSHADERIVPROC glGetShaderiv = nullptr;
PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog = nullptr;
PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation = nullptr;
PFNGLUNIFORM1FPROC glUniform1f = nullptr;
PFNGLUNIFORM1IPROC glUniform1i = nullptr;
PFNGLDELETESHADERPROC glDeleteShader = nullptr;
PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv = nullptr;
PFNGLUNIFORM3FVPROC glUniform3fv = nullptr;
void loadOpenGLFunctions() {
    glCreateShader = (PFNGLCREATESHADERPROC)glutGetProcAddress("glCreateShader");
    glShaderSource = (PFNGLSHADERSOURCEPROC)glutGetProcAddress("glShaderSource");
    glCompileShader = (PFNGLCOMPILESHADERPROC)glutGetProcAddress("glCompileShader");
    glCreateProgram = (PFNGLCREATEPROGRAMPROC)glutGetProcAddress("glCreateProgram");
    glAttachShader = (PFNGLATTACHSHADERPROC)glutGetProcAddress("glAttachShader");
    glLinkProgram = (PFNGLLINKPROGRAMPROC)glutGetProcAddress("glLinkProgram");
    glUseProgram = (PFNGLUSEPROGRAMPROC)glutGetProcAddress("glUseProgram");
    glGetProgramiv = (PFNGLGETPROGRAMIVPROC)glutGetProcAddress("glGetProgramiv");
    glGetProgramInfoLog = (PFNGLGETPROGRAMINFOLOGPROC)glutGetProcAddress("glGetProgramInfoLog");
    glGetShaderiv = (PFNGLGETSHADERIVPROC)glutGetProcAddress("glGetShaderiv");
    glGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC)glutGetProcAddress("glGetShaderInfoLog");
    glGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC)glutGetProcAddress("glGetUniformLocation");
    glUniform1f = (PFNGLUNIFORM1FPROC)glutGetProcAddress("glUniform1f");
    glUniform1i = (PFNGLUNIFORM1IPROC)glutGetProcAddress("glUniform1i");
    glDeleteShader = (PFNGLDELETESHADERPROC)glutGetProcAddress("glDeleteShader");
    glUniformMatrix4fv = (PFNGLUNIFORMMATRIX4FVPROC)glutGetProcAddress("glUniformMatrix4fv");
    glUniform3fv = (PFNGLUNIFORM3FVPROC)glutGetProcAddress("glUniform3fv");

    // Check if all functions were loaded
    if (!glCreateShader || !glShaderSource || !glCompileShader || !glCreateProgram ||
        !glAttachShader || !glLinkProgram || !glUseProgram || !glGetProgramiv ||
        !glGetProgramInfoLog || !glGetShaderiv || !glGetShaderInfoLog ||
        !glGetUniformLocation || !glUniform1f || !glUniform1i) {
        std::cerr << "Failed to load OpenGL functions!" << std::endl;
        exit(EXIT_FAILURE);
    }
}

// Constructor: Compiles and links shaders
Shader::Shader(const char* vertexCode, const char* fragmentCode) {
    
    // Compile shaders
    GLuint vertex = compileShader(GL_VERTEX_SHADER, vertexCode);
    GLuint fragment = compileShader(GL_FRAGMENT_SHADER, fragmentCode);

    // Link shaders into a program
    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);

    // Check for linking errors
    GLint success;
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(ID, 512, nullptr, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    // Clean up
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

// Activate the shader program
void Shader::use() {
    glUseProgram(ID);
}

// Helper function to compile shaders
GLuint Shader::compileShader(GLenum type, const char* code) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &code, nullptr);
    glCompileShader(shader);

    // Check for compilation errors
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cerr << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    return shader;
}

// Set a boolean uniform
void Shader::setBool(const std::string& name, bool value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

// Set an integer uniform
void Shader::setInt(const std::string& name, int value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

// Set a float uniform
void Shader::setFloat(const std::string& name, float value) const {
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setVec3(const std::string& name, const glm::vec3& value) const {
    glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(value));
}

// Set a 4x4 matrix uniform
void Shader::setMat4(const std::string& name, const float* value) const {
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, value);
}