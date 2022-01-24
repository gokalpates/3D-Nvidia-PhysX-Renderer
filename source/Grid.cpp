#include "Grid.h"

Grid::Grid()
{
	setupBuffers();
}

void Grid::draw(Shader& shader)
{
	glBindVertexArray(VAO);

	glUniform3f(glGetUniformLocation(shader.ID, "color"), 1.f, 0.f, 0.f);
	glDrawArrays(GL_LINES, 0, 2);

	glUniform3f(glGetUniformLocation(shader.ID, "color"), 0.f, 1.f, 0.f);
	glDrawArrays(GL_LINES, 2, 2);

	glUniform3f(glGetUniformLocation(shader.ID, "color"), 0.f, 0.f, 1.f);
	glDrawArrays(GL_LINES, 4, 2);
}

void Grid::setupBuffers()
{
	std::vector<float> vertices = {
		// X axis.
		-100.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,
		100.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,
		// Y axis.
		0.f,-100.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,
		0.f,100.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,
		// Z axis.
		0.f,0.f,-100.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,
		0.f,0.f,100.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(0));

	glBindVertexArray(0);
}