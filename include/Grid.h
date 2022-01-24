#pragma once

#include "Shader.h"

class Grid
{
public:
	Grid();

	void draw(Shader& shader);
private:
	void setupBuffers();

	unsigned int VAO, VBO;
};
