#include "Shader.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <GL/glew.h>

Shader::Shader(const std::string &filepath) 
	: m_FilePath(filepath), m_RendererID(0) 
{
	ShaderProgramSource source = ParseShader(filepath);
	m_RendererID = CreateShader(source.VertexSource, source.FragmentSource);
}

Shader::~Shader() {
	glDeleteProgram(m_RendererID);
}

void Shader::Bind() const {
	glUseProgram(m_RendererID);
}

void Shader::Unbind() const {
	glUseProgram(0);
}

void Shader::SetUniform1i(const std::string &name, int v0)
{
	glUniform1i(GetUniformLocation(name), v0);
}

//Set uniforms
void Shader::SetUniform4f(const std::string &name, float v0, float v1, float v2, float v3) {
	glUniform4f(GetUniformLocation(name), v0, v1, v2, v3); //Set shader variable data
}

void Shader::SetUniformMat4f(const std::string &name, const glm::mat4 &matrix)
{
	glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]);
}

int Shader::GetUniformLocation(const std::string &name) {
	if(m_UniformLocationCache.find(name) != m_UniformLocationCache.end()) {
		return m_UniformLocationCache[name];
	}
	
	int location = glGetUniformLocation(m_RendererID, name.c_str()); //Get location of the variable in the shader
	if(location == -1) {
		std::cout << "Warning: uniform '" << name << "' doesn't exist or isn't used!" << std::endl;
	}

	m_UniformLocationCache[name] = location;
	return location;
}

ShaderProgramSource Shader::ParseShader(const std::string &filepath) {
	std::ifstream stream(filepath);
	enum class ShaderType {
		NONE = -1,
		VERTEX = 0,
		FRAGMENT = 1
	};
	std::string line;
	std::stringstream ss[2];
	ShaderType type = ShaderType::NONE;
	while (getline(stream, line)) {
		if(line.find("#shader") != std::string::npos) {
			if(line.find("vertex") != std::string::npos) {
				type = ShaderType::VERTEX;
			}
			else if (line.find("fragment") != std::string::npos) {
				type = ShaderType::FRAGMENT;
			}
		}
		else {
			ss[(int)type] << line << '\n';
		}
	}
	
	return {ss[0].str(), ss[1].str()};
}

unsigned int Shader::CreateShader(const std::string &vertexShader, const std::string &fragmentShader) {
	unsigned int program = glCreateProgram(); //A program is an object to which we can attach shaders
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	//Attack the shader
	glAttachShader(program, vs);
	glAttachShader(program, fs);
	//Link program
	glLinkProgram(program);
	//Validate program
	glValidateProgram(program);
	//Free up resources
	glDeleteShader(vs);
	glDeleteShader(fs);
	//Return reference to the program
	return program;
}

unsigned int Shader::CompileShader(unsigned int type, const std::string &source) {
	unsigned int id = glCreateShader(type); //Create a shader object which will contain source code
	const char *src = source.c_str(); //Change c++ string to c string
	//Put the source code in the shader (shader reference, how many shaders, pointer to pointer to string, if length is NULL then it assumed that the string is null-terminated)
	glShaderSource(id, 1, &src, nullptr); 
	//Compile the shader
	glCompileShader(id);

	//Check errors
	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result); //Compile status
	if (result == GL_FALSE) { //TRUE if shader is flagged for deletion(everything is ok), FALSE if not
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length); //Info log length returned
		char *message = (char*)alloca(length * sizeof(char)); 
		glGetShaderInfoLog(id, length, &length, message); //returns info log (shader, buffer size, returns length of the string returned, pointer to string)
		std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << "shader" << std::endl;
		std::cout << message << std::endl;
		glDeleteShader(id);
		return 0;
	}

	return id;
}