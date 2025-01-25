#include "Engine.h"
#include "BitmapHandler.h"
#include "Cube.h"
#include "Wall.h"

//statyczne zmienne do zmiany
bool Engine::isPerspective = true;
int Engine::windowWidth = 800;
int Engine::windowHeight = 600;
Observer* Engine::observer = nullptr;
static bool isMousePressed = false;
static int lastMouseX = -1;
static int lastMouseY = -1;
static GLenum shadingMode = GL_SMOOTH;

Observer* observer = nullptr;

Cube* texturedCube = nullptr;
std::vector<Cube*> cubes;

float rotationAngle = 0.0f;
Wall* testWall = nullptr;

Wall* testFloor = nullptr;

Wall* testCeiling = nullptr;

GLfloat light1Position[] = { -10.0f, 10.0f, 10.0f, 1.0f };

GLuint depthMapFBO;
GLuint depthMap;
const unsigned int shadowWidth = 1024;  // Shadow map resolution
const unsigned int shadowHeight = 1024;
glm::mat4 viewMatrix;
glm::mat4 projectionMatrix;
glm::mat4 lightSpaceMatrix;// Transformation from world to light space
void updateLightSpaceMatrix();
Shader standardShader;
Shader shadowShader;
void renderScene();
void renderDebugQuad(GLuint texture);



// Updated Shadow Vertex Shader
const char* shadowVertexShaderCode = R"(
#version 330 core
layout(location = 0) in vec3 aPos;
uniform mat4 lightSpaceMatrix;
uniform mat4 model;
void main() {
    gl_Position = lightSpaceMatrix * model * vec4(aPos, 1.0);
}
)";

// Updated Shadow Fragment Shader
const char* shadowFragmentShaderCode = R"(
#version 330 core
void main() {
    // Depth-only rendering
}
)";

// Updated Standard Vertex Shader
const char* standardVertexShaderCode = R"(
#version 330 core
layout(location = 0) in vec3 aPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightSpaceMatrix;

out vec4 FragPosLightSpace;

void main() {
    FragPosLightSpace = lightSpaceMatrix * model * vec4(aPos, 1.0);
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}
)";

// Updated Standard Fragment Shader
const char* standardFragmentShaderCode = R"(
#version 330 core
in vec4 FragPosLightSpace;
uniform sampler2D shadowMap;
out vec4 FragColor;

float calculateShadow(vec4 fragPosLightSpace) {
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;

    float closestDepth = texture(shadowMap, projCoords.xy).r;
    float currentDepth = projCoords.z;

    float bias = 0.005;
    return (currentDepth - bias > closestDepth) ? 0.3 : 1.0;
}

void main() {
    float shadow = calculateShadow(FragPosLightSpace);
    FragColor = vec4(vec3(shadow), 1.0);
}
)";



void Engine::initShadowMap() {
    glGenFramebuffers(1, &depthMapFBO);
    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, shadowWidth, shadowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    GLfloat borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cerr << "Framebuffer is not complete!" << std::endl;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Engine::displayCallback() {
    // Shadow Map Pass
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glViewport(0, 0, shadowWidth, shadowHeight);
    glClear(GL_DEPTH_BUFFER_BIT);

    shadowShader.use();
    shadowShader.setMat4("lightSpaceMatrix", glm::value_ptr(lightSpaceMatrix));
    renderScene(); // Render scene for shadow map

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Main Rendering Pass
    glViewport(0, 0, windowWidth, windowHeight);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    standardShader.use();
    standardShader.setMat4("view", glm::value_ptr(observer->getViewMatrix()));
    standardShader.setMat4("projection", glm::value_ptr(projectionMatrix));
    standardShader.setMat4("lightSpaceMatrix", glm::value_ptr(lightSpaceMatrix));
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    standardShader.setInt("shadowMap", 1);

    renderScene(); // Render the final scene with shadows

    glutSwapBuffers();
}

void renderDebugQuad(GLuint texture) {
    glBindTexture(GL_TEXTURE_2D, texture);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex2f(-1.0f, -1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex2f(1.0f, -1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex2f(1.0f, 1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex2f(-1.0f, 1.0f);
    glEnd();
}

void renderScene() {
    glBegin(GL_TRIANGLES);
    glColor3f(1.0f, 0.0f, 0.0f); // Red
    glVertex3f(-0.5f, -0.5f, -1.0f);
    glColor3f(0.0f, 1.0f, 0.0f); // Green
    glVertex3f(0.5f, -0.5f, -1.0f);
    glColor3f(0.0f, 0.0f, 1.0f); // Blue
    glVertex3f(0.0f, 0.5f, -1.0f);
    glEnd();
}

void updateLightSpaceMatrix() {
    glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 1.0f, 7.5f);
    glm::mat4 lightView = glm::lookAt(
        glm::vec3(-2.0f, 4.0f, -1.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f)
    );
    lightSpaceMatrix = lightProjection * lightView;
    std::cout << "Light-Space Matrix:\n" << glm::to_string(lightSpaceMatrix) << std::endl;
}




Engine::Engine(int argc, char** argv, int width, int height, const char* title) {
    glutInit(&argc, argv);
    
    
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(width, height);
    glutCreateWindow(title);
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        std::cerr << "GLEW Initialization failed: " << glewGetErrorString(err) << std::endl;
        //return -1;
    }
    //loadFramebufferFunctions();
    //loadOpenGLFunctions();


    standardShader = Shader(standardVertexShaderCode, standardFragmentShaderCode);

    shadowShader = Shader(shadowVertexShaderCode, shadowFragmentShaderCode);


    initSettings();

    observer = new Observer(glm::vec3(0.0f, 0.0f, 10.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));


    const float cubeColor[] = { 0.5f, 0.5f, 0.5f };
    GLuint woodTexture = BitmapHandler::loadBitmapFromFile("textures/wood.jpg");

    for (int i = -2; i <= 2; ++i) {
        for (int j = -2; j <= 2; ++j) {
            Cube* cube = new Cube(0.8f, i * 2.0f, j * 2.0f, 0.0f, cubeColor);
            cube->setTextureForSide(0, woodTexture); // przod
            cube->setTextureForSide(1, woodTexture); // tyl
            cube->setTextureForSide(2, woodTexture); // lewo
            cube->setTextureForSide(3, woodTexture); // prawo
            cube->setTextureForSide(4, woodTexture); // gora
            cube->setTextureForSide(5, woodTexture); // dol
            cubes.push_back(cube);
        }
    }

    GLuint wallTexture = BitmapHandler::loadBitmapFromFile("textures/wall.jpg");
    testWall = new Wall(-5.0f, -5.0f, -15.0f, 15.0f, 15.0f, wallTexture);
    testWall->rotateAround(90, glm::vec3(1.0f, 0.0f, 0.0f));

    testFloor = new Wall(0.0f, -10.0f, 0.0f, 15.0f, 15.0f, wallTexture);
    testFloor->rotateAround(90, glm::vec3(1.0f, 0.0f, 0.0f));

    //testCeiling = new Wall(0.0f, 15.0f, 0.0f, 15.0f, 15.0f, wallTexture);
    //testCeiling->rotate(90, glm::vec3(1.0f, 1.0f, -1.0f));

    glutDisplayFunc(displayCallback);
    glutKeyboardFunc(keyboardCallback);
    glutReshapeFunc(reshapeCallback);
    glutMouseFunc(mouseCallback);
    glutMotionFunc(mouseMotionCallback);
    glutTimerFunc(1000 / 60, timerCallback, 0);
}

Engine::~Engine() {
    delete observer;
    for (Cube* cube : cubes) {
        delete cube;
    }
    delete testWall;
}



void Engine::initSettings() {
    glEnable(GL_DEPTH_TEST);
    setClearColor(0.0f, 0.0f, 0.0f);

    initLighting();
    glShadeModel(shadingMode);
    initShadowMap();
}



void Engine::initLighting() {
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    GLfloat ambientLight[] = { 0.05f, 0.05f, 0.05f, 1.0f };
    GLfloat diffuseLight[] = { 0.8f, 0.8f, 0.8f, 1.0f };
    GLfloat specularLight[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat lightPosition[] = { 5.0f, 10.0f, 5.0f, 1.0f };

    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);


    glEnable(GL_LIGHT1);
    GLfloat light1Position[] = { 0.0f, 10.0f, 0.0f, 1.0f };
    GLfloat light1Diffuse[] = { 1.0f, 0.8f, 0.6f, 1.0f };
    GLfloat light1Specular[] = { 1.0f, 0.9f, 0.7f, 1.0f };
    GLfloat spotDirection[] = { 0.0f, -1.0f, 0.0f };

    glLightfv(GL_LIGHT1, GL_POSITION, light1Position);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light1Diffuse);
    glLightfv(GL_LIGHT1, GL_SPECULAR, light1Specular);
    glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 30.0f);
    glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spotDirection);
    glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 10.0f);

    
    glEnable(GL_COLOR_MATERIAL);
}




void Engine::keyboardCallback(unsigned char key, int x, int y) {
    float speed = 0.5f;
    if (key == 'w') {
        observer->moveForward(speed);
    }
    else if (key == 's') {
        observer->moveForward(-speed);
    }
    else if (key == 'a') {
        observer->moveRight(-speed);
    }
    else if (key == 'd') {
        observer->moveRight(speed);
    }
    else if (key == 'q') {
        observer->translate(glm::vec3(0.0f, speed, 0.0f));
    }
    else if (key == 'e') {
        observer->translate(glm::vec3(0.0f, -speed, 0.0f));
    }
    else if (key == 'i') {
        testWall->translate(glm::vec3(0.0f, 0.0f, -speed));
    }
    else if (key == 'k') {
        testWall->translate(glm::vec3(0.0f, 0.0f, speed));
    }
    else if (key == 'j') {
        testWall->translate(glm::vec3(-speed, 0.0f, 0.0f));
    }
    else if (key == 'l') {
        testWall->translate(glm::vec3(speed, 0.0f, 0.0f));
    }
    else if (key == 27) { // ESC
        exit(0);
    }
    else if (key == 'f' || key == 'F') {
        shadingMode = GL_FLAT;
        glShadeModel(shadingMode);
        std::cout << "Flat Shading" << std::endl;
    }
    else if (key == 'g' || key == 'G') {
        shadingMode = GL_SMOOTH;
        glShadeModel(shadingMode);
        std::cout << "Gouraud Shading" << std::endl;
    }
    else if (key == 'b') {
        glm::vec3 point = observer->getPosition();
        float cubeColor[] = { 0.5f, 0.5f, 0.5f };
        Cube* cube = new Cube(1.0, point.x, point.y, point.z, cubeColor);
        glm::vec3 direction = 3.0f * glm::normalize(observer->getTarget() - point);

        cube->translate(direction);
        cubes.push_back(cube);

    }

    glutPostRedisplay();
}

void Engine::mouseCallback(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            isMousePressed = true;
            lastMouseX = x;
            lastMouseY = y;
        }
        else if (state == GLUT_UP) {
            isMousePressed = false;
            lastMouseX = -1;
            lastMouseY = -1;
        }
    }
}

void Engine::mouseMotionCallback(int x, int y) {
    if (!isMousePressed || lastMouseX == -1 || lastMouseY == -1) {
        return;
    }

    int deltaX = x - lastMouseX;
    int deltaY = y - lastMouseY;
    glm::vec3 point = observer->getPosition();
    observer->translate(-point);
    observer->rotate(deltaX * 0.1f, glm::vec3(0.0f, 1.0f, 0.0f));
    observer->translate(point);
    observer->translate(-point);
    observer->rotate(deltaY * 0.1f, glm::vec3(1.0f, 0.0f, 0.0f));
    observer->translate(point);
    lastMouseX = x;
    lastMouseY = y;

    glutPostRedisplay();
}

void Engine::reshapeCallback(int w, int h) {
    glViewport(0, 0, w, h);
    updateProjectionMatrix();
}

void Engine::timerCallback(int value) {
    rotationAngle += 1.0f;
    if (rotationAngle >= 360.0f) {
        rotationAngle -= 360.0f;
    }




    for (int i = 0; i < cubes.size(); i++) {
        glm::vec3 axis = glm::vec3(0.0f, 1.0f, 0.0f);
        glm::vec3 point = glm::vec3(i * 0.2f, 0.0f, i * 0.1f);
        //cubes[i]->rotatePoint(1.0f, axis, point);
    }
    glutPostRedisplay();
    glutTimerFunc(1000 / 60, timerCallback, value); // 60 FPS
}

void Engine::updateProjectionMatrix() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    if (isPerspective) {
        gluPerspective(60.0, static_cast<double>(windowWidth) / windowHeight, 1.0, 100.0);
    }
    else {
        glOrtho(-10.0, 10.0, -10.0, 10.0, 1.0, 100.0);
    }

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void Engine::setClearColor(float r, float g, float b, float a) {
    glClearColor(r, g, b, a);
}

void Engine::start() {
    glutMainLoop();
}