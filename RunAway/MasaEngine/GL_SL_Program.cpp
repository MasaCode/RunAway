#include "GL_SL_Program.h"
#include "MasaEngineError.h"
#include "IOManager.h"

#include <fstream>
#include <vector>

using std::ios;

namespace MasaEngine{

	GL_SL_Program::GL_SL_Program() : _programID(0), _vertexShaderID(0), _fragmentShaderID(0), _numAttribute(0)
	{
	}


	GL_SL_Program::~GL_SL_Program()
	{
	}

	void GL_SL_Program::compileShader(const std::string &vertexShardersFilePath, const std::string &fragmentShaderFilePath){
		
		std::string vertexSource;
		std::string fragSource;

		IOManager::readFileToBuffer(vertexShardersFilePath, vertexSource);
		IOManager::readFileToBuffer(fragmentShaderFilePath, fragSource);

		compileShaderFromSource(vertexSource.c_str(), fragSource.c_str());
	}

	void GL_SL_Program::compileShaderFromSource(const char* vertexSource, const char* fragmentSource){
		_programID = glCreateProgram();

		//To creat the shaders
		_vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
		if (_vertexShaderID == 0)
			fatalError("Vertex shader failed to be created!");

		_fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
		if (_fragmentShaderID == 0)
			fatalError("Fragment shader failed to be created!");
		//finish creating shaders


		compileShaderFile(vertexSource, "Vertex Shader", _vertexShaderID);
		compileShaderFile(fragmentSource, "Fragment Shader", _fragmentShaderID);
	}


	void GL_SL_Program::LinkShaders(){
		//Attach our shaders to our program
		glAttachShader(_programID, _vertexShaderID);
		glAttachShader(_programID, _fragmentShaderID);

		//Link our program
		glLinkProgram(_programID);

		//Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(_programID, GL_LINK_STATUS, (int *)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(_programID, GL_INFO_LOG_LENGTH, &maxLength);

			//The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(_programID, maxLength, &maxLength, &infoLog[0]);

			//We don't need the program anymore.
			glDeleteProgram(_programID);
			//Don't leak shaders either.
			glDeleteShader(_vertexShaderID);
			glDeleteShader(_fragmentShaderID);

			std::printf("%s\n", &(infoLog[0]));
			fatalError("Shader failed to Link!");

			return;
		}

		//Always detach shaders after a successful link.
		glDetachShader(_programID, _vertexShaderID);
		glDetachShader(_programID, _fragmentShaderID);

		glDeleteShader(_vertexShaderID);
		glDeleteShader(_fragmentShaderID);
	}

	GLint GL_SL_Program::getUniformLocation(const std::string& uniformName){
		GLint location;
		location = glGetUniformLocation(_programID, uniformName.c_str());
		if (location == GL_INVALID_INDEX)
			fatalError("Uniform " + uniformName + " not found in shader!");

		return location;
	}


	void GL_SL_Program::addAttribute(const std::string attributeName){
		glBindAttribLocation(_programID, _numAttribute, attributeName.c_str());
		_numAttribute++;
	}

	void GL_SL_Program::use(){
		glUseProgram(_programID);
		for (int i = 0; i < _numAttribute; i++)
			glEnableVertexAttribArray(i);
	}

	void GL_SL_Program::unuse(){
		glUseProgram(0);
		for (int i = 0; i < _numAttribute; i++)
			glDisableVertexAttribArray(i);
	}

	void GL_SL_Program::dispose(){
		if (_programID != 0)
		glDeleteProgram(_programID);
	}

	void GL_SL_Program::compileShaderFile(const char* source, const std::string& filePath, GLuint id){

		glShaderSource(id, 1, &source, nullptr); //1 means i just pass one string variables.

		glCompileShader(id);

		GLint success = 0;
		glGetShaderiv(id, GL_COMPILE_STATUS, &success);
		if (success == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(id, GL_INFO_LOG_LENGTH, &maxLength);

			//The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(id, maxLength, &maxLength, &infoLog[0]);

			//We don't need the shader anymore.
			glDeleteShader(id);

			std::printf("%s\n", &(infoLog[0]));
			fatalError("Shader " + filePath + " failed to compile!");

			return;
		}

	}

}