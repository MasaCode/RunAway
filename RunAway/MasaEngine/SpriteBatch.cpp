#include "SpriteBatch.h"
#include <algorithm>


//New Sprite Batch
namespace MasaEngine{
///////////////////////////////////////////////////////// Glyph Class ///////////////////////////////////////////////////////

	//Default constructor
	Glyph::Glyph(const glm::vec4& DestRect, const glm::vec4& UVRect, GLuint Texture, float Depth, const Color& Color)
		: texture(Texture), depth(Depth)
	{
		topLeft.color = Color;
		topLeft.setPosition(DestRect.x, DestRect.y + DestRect.w);
		topLeft.setUV(UVRect.x, UVRect.y + UVRect.w);

		bottomLeft.color = Color;
		bottomLeft.setPosition(DestRect.x, DestRect.y);
		bottomLeft.setUV(UVRect.x, UVRect.y);

		bottomRight.color = Color;
		bottomRight.setPosition(DestRect.x + DestRect.z, DestRect.y);
		bottomRight.setUV(UVRect.x + UVRect.z, UVRect.y);

		topRight.color = Color;
		topRight.setPosition(DestRect.x + DestRect.z, DestRect.y + DestRect.w);
		topRight.setUV(UVRect.x + UVRect.z, UVRect.y + UVRect.w);
	}

	//Overload constructor
	Glyph::Glyph(const glm::vec4& DestRect, const glm::vec4& UVRect, GLuint Texture, float Depth, const Color& Color,float angle)
		: texture(Texture), depth(Depth)
	{
		
		glm::vec2 halfDims(DestRect.z / 2.0f, DestRect.w / 2.0f);

		
		//Get points centered at origin
		glm::vec2 TL(-halfDims.x, halfDims.y);
		glm::vec2 BL(-halfDims.x, -halfDims.y);
		glm::vec2 BR(halfDims.x, -halfDims.y);
		glm::vec2 TR(halfDims.x, halfDims.y);
		
		


		//Rotate the points
		TL = rotatePoint(TL, angle) + halfDims;
		BL = rotatePoint(BL, angle) + halfDims;
		BR = rotatePoint(BR, angle) + halfDims;
		TR = rotatePoint(TR, angle) + halfDims;

		
		topLeft.color = Color;
		topLeft.setPosition(DestRect.x + TL.x, DestRect.y + TL.y);
		topLeft.setUV(UVRect.x, UVRect.y + UVRect.w);

		bottomLeft.color = Color;
		bottomLeft.setPosition(DestRect.x + BL.x, DestRect.y + BL.y);
		bottomLeft.setUV(UVRect.x, UVRect.y);

		bottomRight.color = Color;
		bottomRight.setPosition(DestRect.x + BR.x, DestRect.y + BR.y);
		bottomRight.setUV(UVRect.x + UVRect.z, UVRect.y);

		topRight.color = Color;
		topRight.setPosition(DestRect.x + TR.x, DestRect.y + TR.y);
		topRight.setUV(UVRect.x + UVRect.z, UVRect.y + UVRect.w);
		
	}


	glm::vec2 Glyph::rotatePoint(glm::vec2& position, float angle){
		glm::vec2 newVector;
		newVector.x = position.x * cos(angle) - position.y * sin(angle);
		newVector.y = position.x * sin(angle) + position.y * cos(angle);
		return newVector;
	}


///////////////////////////////////////////////////// SpriteBatch Class /////////////////////////////////////////////////////

	SpriteBatch::SpriteBatch() :_vbo(0), _vao(0)
	{
	}


	SpriteBatch::~SpriteBatch()
	{
	}

	void SpriteBatch::init(){
		createVertexArray();
	}

	void SpriteBatch::dispose(){
		if (_vao != 0){
			glDeleteVertexArrays(1, &_vao);
			_vao = 0;
		}
		if (_vbo != 0){
			glDeleteBuffers(1, &_vbo);
			_vbo = 0;
		}
	}

	void SpriteBatch::begin(GlyphSortType sortType){
		_sortType = sortType;
		_renderBatches.clear();

		_glyphs.clear();
	}

	void SpriteBatch::end(){
		
		//set up all pointers for fast sorting
		_glyphsPointers.resize(_glyphs.size());
		for (size_t i = 0; i < _glyphs.size(); i++){
			_glyphsPointers[i] = &_glyphs[i];

		}

		sortGlyphs();
		createRenderBatches();
	}

	void SpriteBatch::draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, const Color& color){

		_glyphs.emplace_back(destRect, uvRect, texture, depth, color);

	}

	void SpriteBatch::draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, const Color& color, float angle){
		_glyphs.emplace_back(destRect, uvRect, texture, depth, color,angle);
	}

	void SpriteBatch::draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, const Color& color, const glm::vec2& direction){
		const glm::vec2 right(1.0f, 0.0f);
		
		float angle = acos(glm::dot(right, direction));
		if (direction.y < 0.0f){
			angle = -angle;
		}

		_glyphs.emplace_back(destRect, uvRect, texture, depth, color,angle);
	}



	void SpriteBatch::renderBatch(){

		glBindVertexArray(_vao);

		for (size_t i = 0; i < _renderBatches.size(); i++){
			glBindTexture(GL_TEXTURE_2D, _renderBatches[i].texture);

			glDrawArrays(GL_TRIANGLES, _renderBatches[i].offset, _renderBatches[i].numVertices);
		}

		glBindVertexArray(0);
	}

	void SpriteBatch::createRenderBatches(){
		std::vector<Vertex> vertices;
		vertices.resize(_glyphsPointers.size() * 6);


		if (_glyphsPointers.empty())
			return;

		int offset = 0;
		int currentVertex = 0;
		_renderBatches.emplace_back(offset, 6, _glyphsPointers[0]->texture); // 6 means two triangles * 3 points.

		vertices[currentVertex++] = _glyphsPointers[0]->topLeft; // after this line currentVertexx is increasing
		vertices[currentVertex++] = _glyphsPointers[0]->bottomLeft;
		vertices[currentVertex++] = _glyphsPointers[0]->bottomRight;

		vertices[currentVertex++] = _glyphsPointers[0]->bottomRight;
		vertices[currentVertex++] = _glyphsPointers[0]->topRight;
		vertices[currentVertex++] = _glyphsPointers[0]->topLeft;

		offset += 6;

		for (size_t currentGlhyph = 1; currentGlhyph < _glyphsPointers.size(); currentGlhyph++){
			if (_glyphsPointers[currentGlhyph]->texture != _glyphsPointers[currentGlhyph - 1]->texture){
				_renderBatches.emplace_back(offset, 6, _glyphsPointers[currentGlhyph]->texture);
			}
			else{
				_renderBatches.back().numVertices += 6;
			}

			vertices[currentVertex++] = _glyphsPointers[currentGlhyph]->topLeft;
			vertices[currentVertex++] = _glyphsPointers[currentGlhyph]->bottomLeft;
			vertices[currentVertex++] = _glyphsPointers[currentGlhyph]->bottomRight;

			vertices[currentVertex++] = _glyphsPointers[currentGlhyph]->bottomRight;
			vertices[currentVertex++] = _glyphsPointers[currentGlhyph]->topRight;
			vertices[currentVertex++] = _glyphsPointers[currentGlhyph]->topLeft;

			offset += 6;
		}

		glBindBuffer(GL_ARRAY_BUFFER, _vbo);
		//orphan the buffer
		
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);
		//upload the data

		glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() *sizeof(Vertex), vertices.data());

		glBindBuffer(GL_ARRAY_BUFFER, 0);

	}


	void SpriteBatch::createVertexArray(){

		if (_vao == 0)
			glGenVertexArrays(1, &_vao); // vertex array object

		glBindVertexArray(_vao);

		if (_vbo == 0)
			glGenBuffers(1, &_vbo); // vertex buffer object

		glBindBuffer(GL_ARRAY_BUFFER, _vbo);

		glEnableVertexAttribArray(0);

		glEnableVertexAttribArray(1);

		glEnableVertexAttribArray(2);

		//This is the position attribute pointer
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position)); // 2 is for x and y
		//This is the color attribute pointer.
		glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, color));
		//This is the UV attribute pointer;
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, uv));



		glBindVertexArray(0);

	}

	void SpriteBatch::sortGlyphs(){

		switch (_sortType){
			case GlyphSortType::BACK_TO_FRONT:
				std::stable_sort(_glyphsPointers.begin(), _glyphsPointers.end(), compareBackToFront);//It may be correct
				break;
			case GlyphSortType::FRONT_TO_BACK:
				std::stable_sort(_glyphsPointers.begin(), _glyphsPointers.end(), compareFrontToBack);
				break;
			case GlyphSortType::TEXTURE:
				std::stable_sort(_glyphsPointers.begin(), _glyphsPointers.end(), compareTexture);
				break;
		}

	}


	bool SpriteBatch::compareFrontToBack(Glyph* a, Glyph* b){
		return (a->depth < b->depth);
	}

	bool SpriteBatch::compareBackToFront(Glyph* a, Glyph* b){
		return (a->depth > b->depth);
	}

	bool SpriteBatch::compareTexture(Glyph* a, Glyph* b){
		return (a->texture < b->texture);
	}

}

