#pragma once

#include "GL_SL_Program.h"
#include "Vertex.h"
#include <glm/glm.hpp>
#include <vector>


namespace MasaEngine{

	struct DebugVertex{
		glm::vec2 position;
		MasaEngine::Color color;
	};


	class DebugRenderer
	{
	public:
		DebugRenderer();
		~DebugRenderer();

		void init();
		void end();

		void drawLine(const glm::vec2& from, const glm::vec2& to, const Color& color);
		void drawBox(const glm::vec4& destRect, const Color& color, float angle);
		void drawCircle(const glm::vec2& center, const Color& color, float radius);
		void drawTriangle(const glm::vec4& destRect, const Color& color, float angle);

		void render(const glm::mat4& projectionMatrix,float lineWidth = 1.0f);
		void dispose();

	private:
		MasaEngine::GL_SL_Program _program;
		std::vector<DebugVertex> _verts;
		std::vector<GLuint> _indices;
		//Vertext Buffer Object, Vertex Array Object, Index Buffer Object
		GLuint _vbo = 0, _vao = 0, _ibo = 0;

		int _numElement = 0;
	};

}