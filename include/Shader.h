#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <filesystem>

class Shader
{
public:
    unsigned int ID;
    // constructor generates the shader on the fly
    // ------------------------------------------------------------------------
	Shader(std::filesystem::path vertexShaderPath, std::filesystem::path fragmentShaderPath) :
		ID(0u)
	{
		std::ifstream fileStream(vertexShaderPath);
		if (!fileStream.is_open())
		{
			std::cout << "ERROR: Program could not open: " << vertexShaderPath << "\n";
			glfwTerminate();
			std::exit(EXIT_FAILURE);
		}
		std::stringstream vertexStream;
		vertexStream << fileStream.rdbuf();
		std::string vertexShaderSource = vertexStream.str();
		const char* vertexSource = vertexShaderSource.c_str();
		fileStream.close();

		fileStream.open(fragmentShaderPath);
		if (!fileStream.is_open())
		{
			std::cout << "ERROR: Program could not open: " << fragmentShaderPath << "\n";
			glfwTerminate();
			std::exit(EXIT_FAILURE);
		}
		std::stringstream fragmentStream;
		fragmentStream << fileStream.rdbuf();
		std::string fragmentShaderSource = fragmentStream.str();
		const char* framentSource = fragmentShaderSource.c_str();
		fileStream.close();

		unsigned int vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShaderId, 1, &vertexSource, nullptr);
		glCompileShader(vertexShaderId);
		int success = 0;
		glGetShaderiv(vertexShaderId, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			char infoLog[512];
			glGetShaderInfoLog(vertexShaderId, 512, nullptr, infoLog);
			std::cout << "ERROR: OpenGL could not compile vertex shader: " << infoLog << "\n";
			glfwTerminate();
			std::exit(EXIT_FAILURE);
		}

		unsigned int fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShaderId, 1, &framentSource, nullptr);
		glCompileShader(fragmentShaderId);
		success = 0;
		glGetShaderiv(fragmentShaderId, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			char infoLog[512];
			glGetShaderInfoLog(fragmentShaderId, 512, nullptr, infoLog);
			std::cout << "ERROR: OpenGL could not compile fragment shader: " << infoLog << "\n";
			glfwTerminate();
			std::exit(EXIT_FAILURE);
		}

		ID = glCreateProgram();
		glAttachShader(ID, vertexShaderId);
		glAttachShader(ID, fragmentShaderId);
		glLinkProgram(ID);
		success = 0;
		glGetProgramiv(ID, GL_LINK_STATUS, &success);
		if (!success)
		{
			char infoLog[512];
			glGetProgramInfoLog(ID, 512, nullptr, infoLog);
			std::cout << "ERROR: OpenGL could not link vertex and fragment shaders: " << infoLog << "\n";
			glfwTerminate();
			std::exit(EXIT_FAILURE);
		}

		glDeleteShader(vertexShaderId);
		glDeleteShader(fragmentShaderId);
	}

	Shader(std::filesystem::path vertexShaderPath, std::filesystem::path geometryShaderPath, std::filesystem::path fragmentShaderPath) :
		ID(0u)
	{
		std::ifstream fileStream(vertexShaderPath);
		if (!fileStream.is_open())
		{
			std::cout << "ERROR: Program could not open: " << vertexShaderPath << "\n";
			glfwTerminate();
			std::exit(EXIT_FAILURE);
		}
		std::stringstream vertexStream;
		vertexStream << fileStream.rdbuf();
		std::string vertexShaderSource = vertexStream.str();
		const char* vertexSource = vertexShaderSource.c_str();
		fileStream.close();

		fileStream.open(geometryShaderPath);
		if (!fileStream.is_open())
		{
			std::cout << "ERROR: Program could not open: " << vertexShaderPath << "\n";
			glfwTerminate();
			std::exit(EXIT_FAILURE);
		}
		std::stringstream geometryStream;
		geometryStream << fileStream.rdbuf();
		std::string geometryShaderSource = geometryStream.str();
		const char* geometrySource = geometryShaderSource.c_str();
		fileStream.close();

		fileStream.open(fragmentShaderPath);
		if (!fileStream.is_open())
		{
			std::cout << "ERROR: Program could not open: " << fragmentShaderPath << "\n";
			glfwTerminate();
			std::exit(EXIT_FAILURE);
		}
		std::stringstream fragmentStream;
		fragmentStream << fileStream.rdbuf();
		std::string fragmentShaderSource = fragmentStream.str();
		const char* framentSource = fragmentShaderSource.c_str();
		fileStream.close();

		unsigned int vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShaderId, 1, &vertexSource, nullptr);
		glCompileShader(vertexShaderId);
		int success = 0;
		glGetShaderiv(vertexShaderId, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			char infoLog[512];
			glGetShaderInfoLog(vertexShaderId, 512, nullptr, infoLog);
			std::cout << "ERROR: OpenGL could not compile vertex shader: " << infoLog << "\n";
			glfwTerminate();
			std::exit(EXIT_FAILURE);
		}

		unsigned int geometryShaderId = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(geometryShaderId, 1, &geometrySource, nullptr);
		glCompileShader(geometryShaderId);
		success = 0;
		glGetShaderiv(geometryShaderId, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			char infoLog[512];
			glGetShaderInfoLog(geometryShaderId, 512, nullptr, infoLog);
			std::cout << "ERROR: OpenGL could not compile geometry shader: " << infoLog << "\n";
			glfwTerminate();
			std::exit(EXIT_FAILURE);
		}

		unsigned int fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShaderId, 1, &framentSource, nullptr);
		glCompileShader(fragmentShaderId);
		success = 0;
		glGetShaderiv(fragmentShaderId, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			char infoLog[512];
			glGetShaderInfoLog(fragmentShaderId, 512, nullptr, infoLog);
			std::cout << "ERROR: OpenGL could not compile fragment shader: " << infoLog << "\n";
			glfwTerminate();
			std::exit(EXIT_FAILURE);
		}

		ID = glCreateProgram();
		glAttachShader(ID, vertexShaderId);
		glAttachShader(ID, geometryShaderId);
		glAttachShader(ID, fragmentShaderId);
		glLinkProgram(ID);
		success = 0;
		glGetProgramiv(ID, GL_LINK_STATUS, &success);
		if (!success)
		{
			char infoLog[512];
			glGetProgramInfoLog(ID, 512, nullptr, infoLog);
			std::cout << "ERROR: OpenGL could not link vertex, geometry and fragment shaders: " << infoLog << "\n";
			glfwTerminate();
			std::exit(EXIT_FAILURE);
		}

		glDeleteShader(vertexShaderId);
		glDeleteShader(geometryShaderId);
		glDeleteShader(fragmentShaderId);
	}

    // activate the shader
    // ------------------------------------------------------------------------
    void use()
    {
        glUseProgram(ID);
    }
    // utility uniform functions
    // ------------------------------------------------------------------------
    void setBool(const std::string& name, bool value) const
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
    }
    // ------------------------------------------------------------------------
    void setInt(const std::string& name, int value) const
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
    }
    // ------------------------------------------------------------------------
    void setFloat(const std::string& name, float value) const
    {
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
    }
    // ------------------------------------------------------------------------
    void setVec2(const std::string& name, const glm::vec2& value) const
    {
        glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
    }
    void setVec2(const std::string& name, float x, float y) const
    {
        glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
    }
    // ------------------------------------------------------------------------
    void setVec3(const std::string& name, const glm::vec3& value) const
    {
        glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
    }
    void setVec3(const std::string& name, float x, float y, float z) const
    {
        glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
    }
    // ------------------------------------------------------------------------
    void setVec4(const std::string& name, const glm::vec4& value) const
    {
        glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
    }
    void setVec4(const std::string& name, float x, float y, float z, float w)
    {
        glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
    }
    // ------------------------------------------------------------------------
    void setMat2(const std::string& name, const glm::mat2& mat) const
    {
        glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
    // ------------------------------------------------------------------------
    void setMat3(const std::string& name, const glm::mat3& mat) const
    {
        glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
    // ------------------------------------------------------------------------
    void setMat4(const std::string& name, const glm::mat4& mat) const
    {
        glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }

private:
    // utility function for checking shader compilation/linking errors.
    // ------------------------------------------------------------------------
    void checkCompileErrors(GLuint shader, std::string type)
    {
        GLint success;
        GLchar infoLog[1024];
        if (type != "PROGRAM")
        {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
        else
        {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success)
            {
                glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
    }
};
