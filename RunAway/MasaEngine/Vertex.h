#pragma once

#include <glew/glew.h>

namespace MasaEngine{

	struct Position{
		float x;
		float y;
	};

	struct Color{
		Color() : r(0), g(0), b(0), a(255) {}
		Color(GLubyte red, GLubyte green, GLubyte blue, GLubyte alpha) : r(red), g(green), b(blue), a(alpha) {}

		GLubyte r; // it is same type as unsigned char.
		GLubyte g; // it is same type as unsigned char.
		GLubyte b; // it is same type as unsigned char.
		GLubyte a; // it is same type as unsigned char.
	};

	struct UV{
		float u;
		float v;
	};

	struct Vertex{

		Position position;

		Color color;
		//GLubyte color[4]; // it is same type as unsigned char. it's to save up the memory. it must be one byte.

		//UV texture coordinates
		UV uv;

		void setPosition(float x, float y){
			position.x = x;
			position.y = y;
		}

		void setColor(GLubyte r, GLubyte g, GLubyte b, GLubyte a){
			color.r = r;
			color.g = g;
			color.b = b;
			color.a = a;
		}

		void setUV(float u, float v){
			uv.u = u;
			uv.v = v;
		}
	};

}