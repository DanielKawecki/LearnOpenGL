
#ifndef SHADER_H
#define SHADER_H

#include <string>

class Shader {

private:
	unsigned int _ID;
	std::string _vertSource;
	std::string _fragSource;

public:
	Shader(const char* vertPath, const char* fragPath);
	~Shader();

	void Compile();
	void Bind() const;
	unsigned int GetID() const;
};

#endif // !SHADER_H
