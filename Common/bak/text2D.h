#ifndef TEXT2D_HPP
#define TEXT2D_HPP
#include "JuceHeader.h"

void initText2D(OpenGLContext context, const char * texturePath);
void printText2D(OpenGLContext context, const char * text, int x, int y, int size);
void cleanupText2D(OpenGLContext context);

#endif