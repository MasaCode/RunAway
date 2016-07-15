#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace MasaEngine{

	class Camera2D
	{
	public:
		Camera2D();
		~Camera2D();

		void init(int screenWidth, int screenHeight);
		void update();

		glm::vec2 convertScreenToWorld(glm::vec2 screenCoords);

		bool isBoxInView(const glm::vec2& position, const glm::vec2 dimentions);

		void offsetPosition(const glm::vec2& offset){ _position += offset; _needsMatrixUpdate = true; }
		void offsetScale(float offset){ _scale += offset; if (_scale < 0.001f) _scale = 0.001f; _needsMatrixUpdate = true; }

		//setters
		void setPosition(const glm::vec2& newPosition){ _position = newPosition; _needsMatrixUpdate = true; }
		void setScale(float newScale){ _scale = newScale; _needsMatrixUpdate = true; }

		//getters
		const glm::vec2& getPosition(){ return _position; }
		float getScale() const { return _scale; }
		const glm::mat4 getCameraMatrix() const { return _cameraMatrix; }
		float getAspectRatio() const { return (float)_screenWidth / (float)_screenHeight; }
		// This is camera width and height.
		glm::vec2& getCameraDimensions() const { return (glm::vec2(_screenWidth, _screenHeight) / _scale); }


	private:
		int _screenWidth, _screenHeight;
		float _scale;
		bool _needsMatrixUpdate;
		glm::vec2 _position;
		glm::mat4 _cameraMatrix;
		glm::mat4 _orthoMatrix;
	};

}