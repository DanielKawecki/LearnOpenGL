#include "shader.h"

#include <glad/glad.h>

#include <fstream>
#include <sstream>
#include <iostream>

Shader::Shader(const char* vertPath, const char* fragPath) {

	_ID = 0;

	std::ifstream vertFile;
	std::ifstream fragFile;

	vertFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fragFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try {
		vertFile.open(vertPath);
		fragFile.open(fragPath);
	
		std::stringstream vertStream, fragStream;

		vertStream << vertFile.rdbuf();
		fragStream << fragFile.rdbuf();

		vertFile.close();
		fragFile.close();

		std::string source;

		_vertSource = vertStream.str();
		_fragSource = fragStream.str();
	}
	catch (std::ifstream::failure e) {
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ\n";
	}
}

Shader::~Shader() {}

void Shader::Compile() {

	unsigned int vert, frag;
	int success;
	char infoLog[512];

	vert = glCreateShader(GL_VERTEX_SHADER);
	frag = glCreateShader(GL_FRAGMENT_SHADER);

	const char* vSrc = _vertSource.c_str();
	const char* fSrc = _fragSource.c_str();

	glShaderSource(vert, 1, &vSrc, NULL);
	glShaderSource(frag, 1, &fSrc, NULL);

	glCompileShader(vert);
	glCompileShader(frag);

	glGetShaderiv(GL_VERTEX_SHADER, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vert, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	glGetShaderiv(GL_FRAGMENT_SHADER, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(frag, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	_ID = glCreateProgram();
	glAttachShader(_ID, vert);
	glAttachShader(_ID, frag);
	glLinkProgram(_ID);

	glGetProgramiv(_ID, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(_ID, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	glDeleteShader(vert);
	glDeleteShader(frag);
}

void Shader::Bind() const {
	glUseProgram(_ID);
}

unsigned int Shader::GetID() const {
	return _ID;
}
