#include "DebugRenderer.h"

const float PI = 3.14159265359f;

namespace {

	const char* VERT_SRC = R"(#version 130

in vec2 vertexPosition;
in vec4 vertexColor;

out vec2 fragmentPosition;
out vec4 fragmentColor;

uniform mat4 P;

void main(){
	
	gl_Position.xy = (P * vec4(vertexPosition,0.0,1.0)).xy;
	
	gl_Position.z = 0.0;
	
	gl_Position.w = 1.0;
	
	fragmentPosition = vertexPosition;
	
	fragmentColor = vertexColor;


})";

	const char* FRAG_SRC = R"(#version 130


in vec2 fragmentPosition;
in vec4 fragmentColor;

out vec4 color;

void main(){

	color = fragmentColor;
	
})";

}

namespace MasaEngine{

	DebugRenderer::DebugRenderer()
	{
		//Empty
	}


	DebugRenderer::~DebugRenderer()
	{
		dispose();
	}

	void DebugRenderer::init(){
		//Shader Initialization.
		_program.compileShaderFromSource(VERT_SRC,FRAG_SRC);
		_program.addAttribute("vertexPosition");
		_program.addAttribute("vertexColor");
		_program.LinkShaders();


		//Set up Buffer
		glGenVertexArrays(1, &_vao);
		glGenBuffers(1, &_vbo);
		glGenBuffers(1, &_ibo); 

		glBindVertexArray(_vao);
		glBindBuffer(GL_ARRAY_BUFFER, _vbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);

		//Set up the attribute pointer.
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(DebugVertex), (void*)offsetof(DebugVertex, position));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(DebugVertex), (void*)offsetof(DebugVertex,color));

		glBindVertexArray(0);

	}

	void DebugRenderer::end(){
		glBindBuffer(GL_ARRAY_BUFFER, _vbo);
		//Orphan the buffer
		glBufferData(GL_ARRAY_BUFFER, _verts.size() * sizeof(DebugVertex),nullptr,GL_DYNAMIC_DRAW);
		//Update the data
		glBufferSubData(GL_ARRAY_BUFFER, 0, _verts.size() * sizeof(DebugVertex), _verts.data());
		glBindBuffer(GL_ARRAY_BUFFER, 0);


		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
		//Orphan the buffer
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices.size() * sizeof(GLuint), nullptr, GL_DYNAMIC_DRAW);
		//Update the data
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, _indices.size() * sizeof(GLuint), _indices.data());
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		_numElement = _indices.size();
		_indices.clear();
		_verts.clear();

	}

	glm::vec2 rotatePoint(glm::vec2& position, float angle){
		glm::vec2 newVector;
		newVector.x = position.x * cos(angle) - position.y * sin(angle);
		newVector.y = position.x * sin(angle) + position.y * cos(angle);
		return newVector;
	}

	void DebugRenderer::drawLine(const glm::vec2& from, const glm::vec2& to, const Color& color){
		int i = _verts.size();
		_verts.resize(_verts.size() + 2);
		
		_verts[i].position = from;
		_verts[i].color = color;

		_verts[i + 1].position = to;
		_verts[i + 1].color = color;

		_indices.push_back(i);
		_indices.push_back(i + 1);

	}

	void DebugRenderer::drawBox(const glm::vec4& destRect, const Color& color, float angle){
		
		int i = _verts.size();
		_verts.resize(_verts.size() + 4);
		
		glm::vec2 halfDims(destRect.z / 2.0f, destRect.w / 2.0f);


		//Get points centered at origin
		glm::vec2 TL(-halfDims.x, halfDims.y);
		glm::vec2 BL(-halfDims.x, -halfDims.y);
		glm::vec2 BR(halfDims.x, -halfDims.y);
		glm::vec2 TR(halfDims.x, halfDims.y);


		glm::vec2 positionOffset(destRect.x, destRect.y);

		//Rotate the points
		_verts[i].position = rotatePoint(TL, angle) + halfDims + positionOffset;
		_verts[i + 1].position = rotatePoint(BL, angle) + halfDims + positionOffset;
		_verts[i + 2].position = rotatePoint(BR, angle) + halfDims + positionOffset;
		_verts[i + 3].position = rotatePoint(TR, angle) + halfDims + positionOffset;

		for (int j = i; j < i + 4; j++){
			_verts[j].color = color;
		}

		
		_indices.reserve(_indices.size() + 8);

		//Line 1
		_indices.push_back(i);
		_indices.push_back(i + 1);

		//Line 2
		_indices.push_back(i + 1);
		_indices.push_back(i + 2);
	
		//Line 3
		_indices.push_back(i + 2);
		_indices.push_back(i + 3);

		//Line 4
		_indices.push_back(i + 3);
		_indices.push_back(i);

	}
	
	void DebugRenderer::drawCircle(const glm::vec2& center, const Color& color, float radius){
		static const int NUM_VERTS = 100;

		//Set up vertices
		int  start = _verts.size();
		_verts.resize(_verts.size() + NUM_VERTS);
		for (int i = 0; i < NUM_VERTS; i++){
			float angle = ((float)i / NUM_VERTS) * PI * 2.0f;

			_verts[start + i].position.x = cos(angle) * radius + center.x;
			_verts[start + i].position.y = sin(angle) * radius + center.y;
			_verts[start + i].color = color;
		}

		//Set up indices for indeed drawing
		_indices.reserve(_indices.size() + NUM_VERTS * 2);
		for (int i = 0; i < NUM_VERTS - 1; i++){
			_indices.push_back(start + i);
			_indices.push_back(start + i + 1);
		}
		_indices.push_back(start + NUM_VERTS - 1);
		_indices.push_back(start);

	}


	void DebugRenderer::drawTriangle(const glm::vec4& destRect, const Color& color, float angle){
		int i = _verts.size();
		_verts.resize(_verts.size() + 3);


		glm::vec2 halfDims(destRect.z / 2.0f, destRect.w / 2.0f);

		glm::vec2 Top(0, halfDims.y);
		glm::vec2 BL(-halfDims.x, -halfDims.y);
		glm::vec2 BR(halfDims.x, -halfDims.y);

		glm::vec2 positionOffset(destRect.x, destRect.y);

		_verts[i].position = rotatePoint(Top, angle) + halfDims + positionOffset;
		_verts[i + 1].position = rotatePoint(BL, angle) + halfDims + positionOffset;
		_verts[i + 2].position = rotatePoint(BR, angle) + halfDims + positionOffset;

		for (int j = i; j < i + 3; j++){
			_verts[j].color = color;
		}


		_indices.reserve(_indices.size() + 6);

		//First line
		_indices.push_back(i);
		_indices.push_back(i + 1);

		//Second line
		_indices.push_back(i + 1);
		_indices.push_back(i + 2);

		//Third line
		_indices.push_back(i + 2);
		_indices.push_back(i);

	}

	
	void DebugRenderer::render(const glm::mat4& projectionMatrix, float lineWidth){
		_program.use();

		GLint pUniform = _program.getUniformLocation("P");
		glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

		glLineWidth(lineWidth);
		glBindVertexArray(_vao);
		glDrawElements(GL_LINES, _numElement, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		_program.unuse();
	}
	
	void DebugRenderer::dispose(){
		if (_vao){
			glDeleteVertexArrays(1, &_vao);
		}
		if (_vbo){
			glDeleteBuffers(1, &_vbo);
		}
		if (_ibo){
			glDeleteBuffers(1, &_ibo);
		}

		_program.dispose();
	}

}