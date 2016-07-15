#pragma once

#include <glew/glew.h>
#include <string>


namespace MasaEngine{

	class GL_SL_Program
	{
	public:
		GL_SL_Program();
		~GL_SL_Program();

		void compileShader(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath);

		void compileShaderFromSource(const char* vertexSource, const char* fragmentSource);

		void LinkShaders();

		void addAttribute(const std::string attributeName);

		GLint getUniformLocation(const std::string& uniformName);

		void use();
		void unuse();

		void dispose();

	private:

		void compileShaderFile(const char* source, const std::string& filePath, GLuint id);


		int _numAttribute;
		GLint _programID;

		GLuint _vertexShaderID;
		GLuint _fragmentShaderID;
	};

}