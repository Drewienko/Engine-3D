#ifndef BITMAPHANDLER_H
#define BITMAPHANDLER_H

#include <GL/freeglut.h>
#include <vector>
#include <string>

class BitmapHandler {
public:

    static GLuint loadBitmapFromFile(const std::string& filename);
    static GLuint createBitmap(int width, int height, unsigned char r, unsigned char g, unsigned char b);
    static void deleteBitmap(GLuint textureID);
    static void copyBitmap(GLuint sourceTextureID, GLuint destinationTextureID, int x, int y, int width, int height);
};

#endif // BITMAPHANDLER_H
