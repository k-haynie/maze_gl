#ifndef SHADER_H
#define SHADER_H

#include "glad.h"
#include <glm/glm.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

class Shader {
public:
	unsigned int ID;

	Shader(const char* vertexPath, const char* fragPath) {
		string vertexCode;
		string fragCode;
		ifstream vShaderFile;
		ifstream fShaderFile;

		vShaderFile.exceptions(ifstream::failbit | ifstream::badbit);
		fShaderFile.exceptions(ifstream::failbit | ifstream::badbit);

		try {
			vShaderFile.open(vertexPath);
			fShaderFile.open(fragPath);
			stringstream vShaderStream, fShaderStream;

			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();

			vShaderFile.close();
			fShaderFile.close();

			vertexCode = vShaderStream.str();
			fragCode = fShaderStream.str();
		} 
		catch (ifstream::failure e) {
			cout << "ERROR: SHADER FILE NOT SUCCESSFULLY READ" << endl;
		}
		const char* vShaderCode = vertexCode.c_str();
		const char* fShaderCode = fragCode.c_str();

		unsigned int vertex, fragment;
		int success;
		char infoLog[512];

		// create the vertex shader
		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vShaderCode, NULL);
		glCompileShader(vertex);
		
		glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(vertex, 512, NULL, infoLog);
			cout << "ERROR: VERTEX SHADER COMPILATION FAILED\n" << infoLog << endl;
		}

		// create the fragment shader
		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fShaderCode, NULL);
		glCompileShader(fragment);

		glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(fragment, 512, NULL, infoLog);
			cout << "ERROR: FRAGMENT SHADER COMPILATION FAILED\n" << infoLog << endl;
		}

		// initialize the shader program
		ID = glCreateProgram();
		glAttachShader(ID, vertex);
		glAttachShader(ID, fragment);
		glLinkProgram(ID);

		glGetProgramiv(ID, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(ID, 512, NULL, infoLog);
			cout << "ERROR: SHADER PROGRAM LINKING FAILED\n" << infoLog << endl;
		}

		glDeleteShader(vertex);
		glDeleteShader(fragment);
	};
	
	void use() {
		glUseProgram(ID);
	};

	void setBool(const string& name, bool value) const {
		glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
	};

	void setInt(const string& name, int value) const {
		glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
	};

	void setFloat(const string& name, float value) const {
		glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
	};

	void setMat4(const string& name, const glm::mat4& mat) const
	{
		glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}

};

#endif