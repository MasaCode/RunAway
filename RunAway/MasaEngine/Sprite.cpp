#include "Sprite.h"
#include "Vertex.h"
#include "ResourceManager.h"

#include <cstddef>

namespace MasaEngine{

	Sprite::Sprite(){
		_vboID = 0;
	}


	Sprite::~Sprite(){
		if (_vboID != 0)
			glDeleteBuffers(1, &_vboID);
	}

	void Sprite::init(float x, float y, float height, float width, std::string texturePath){
		_x = x;
		_y = y;
		_height = height;
		_width = width;

		_texture = ResourceManager::getTexture(texturePath);//To get texture

		if (_vboID == 0)
			glGenBuffers(1, &_vboID); // must be unique. gl-generating-buffers.

		Vertex vertexData[6]; // 6 positions multiply by 2 elements (x and y);

		//first Traiangle
		vertexData[0].setPosition(_x + _width, _y + _height);
		vertexData[0].setUV(1.0f, 1.0f);

		vertexData[1].setPosition(_x, _y + _height);
		vertexData[1].setUV(0.0f, 1.0f);

		vertexData[2].setPosition(_x, _y);
		vertexData[2].setUV(0.0f, 0.0f);

		//second Traiangle
		vertexData[3].setPosition(_x, _y);
		vertexData[3].setUV(0.0f, 0.0f);

		vertexData[4].setPosition(_x + _width, _y);
		vertexData[4].setUV(1.0f, 0.0f);

		vertexData[5].setPosition(_x + _width, _y + _height);
		vertexData[5].setUV(1.0f, 1.0f);


		//setting color to each elements.
		for (int i = 0; i < 6; i++){
			vertexData[i].setColor(255, 0, 255, 255);
		}

		//Playing with color!!!! Interprating.
		vertexData[1].setColor(0, 0, 255, 255);

		vertexData[4].setColor(0, 255, 0, 255);
		//Until here.


		glBindBuffer(GL_ARRAY_BUFFER, _vboID);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, 0); //to safty
	}

	void Sprite::draw(){

		glBindTexture(GL_TEXTURE_2D, _texture.id); //To bind texture

		glBindBuffer(GL_ARRAY_BUFFER, _vboID);

		glEnableVertexAttribArray(0);

		glEnableVertexAttribArray(1);

		glEnableVertexAttribArray(2);

		//This is the position attribute pointer
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position)); // 2 is for x and y
		//This is the color attribute pointer.
		glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, color));
		//This is the UV attribute pointer;
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, uv));

		glDrawArrays(GL_TRIANGLES, 0, 3); //drawing first triangle
		glDrawArrays(GL_TRIANGLES, 3, 6); //drawing second triangle.

		glDisableVertexAttribArray(0);

		glDisableVertexAttribArray(1);

		glDisableVertexAttribArray(2);


		glBindBuffer(GL_ARRAY_BUFFER, 0); //to safty
	}

}