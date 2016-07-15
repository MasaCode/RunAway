#include "Camera2D.h"

#include <algorithm>

namespace MasaEngine{

	Camera2D::Camera2D() :
		_position(0, 0), _cameraMatrix(1.0f), _scale(1.0f), _needsMatrixUpdate(true),
		_screenHeight(500), _screenWidth(500), _orthoMatrix(1.0f)
	{
	}


	Camera2D::~Camera2D()
	{
	}


	void Camera2D::init(int screenWidth, int screenHeight){
		_screenHeight = screenHeight;
		_screenWidth = screenWidth;
		_orthoMatrix = glm::ortho(0.0f, (float)_screenWidth, 0.0f, (float)_screenHeight);
	}


	void Camera2D::update(){

		if (_needsMatrixUpdate){
			//Camera Translation
			glm::vec3 translate(-_position.x + _screenWidth /2, -_position.y + _screenHeight / 2, 0.0f);
			_cameraMatrix = glm::translate(_orthoMatrix, translate);
			
			//Camera Scale
			glm::vec3 scale(_scale, _scale, 0.0f);
			_cameraMatrix = glm::scale(glm::mat4(1.0f), scale) * _cameraMatrix;

			_needsMatrixUpdate = false;
		}

	}

	glm::vec2 Camera2D::convertScreenToWorld(glm::vec2 screenCoords){
		//invert Y direction
		screenCoords.y = _screenHeight - screenCoords.y;

		//Make it so that 0 is the center
		screenCoords -= glm::vec2(_screenWidth / 2, _screenHeight / 2);
		//Scale the coordinates
		screenCoords /= _scale;
		//Translate with the camera position
		screenCoords += _position;
		return screenCoords;
	}

	bool Camera2D::isBoxInView(const glm::vec2& position, const glm::vec2 dimentions){

		glm::vec2 scaleScreenDimensions = glm::vec2(_screenWidth, _screenHeight) / _scale;


		const float MIN_DISTANCE_X = dimentions.x / 2.0f + scaleScreenDimensions.x / 2.0f;
		const float MIN_DISTANCE_Y = dimentions.y / 2.0f + scaleScreenDimensions.y / 2.0f;

		//Center position of the parameter
		glm::vec2 centerPosition = position + dimentions / 2.0f;
		//Center position of the camera. (_position is center)
		glm::vec2 centerCameraPosition = _position;

		glm::vec2 distVec = centerPosition - centerCameraPosition;


		float xDepth = MIN_DISTANCE_X - abs(distVec.x);
		float yDepth = MIN_DISTANCE_Y - abs(distVec.y);

		//if this is true, we are colliding with tile
		if (xDepth > 0 && yDepth > 0){
			//There was a collision
			return true;
		}


		return false;
	}

}