#pragma once

#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

float alip(float a, float b, float f);
void renderCube();
void renderQuad();
unsigned int loadTextureFromFile(const char* path);
