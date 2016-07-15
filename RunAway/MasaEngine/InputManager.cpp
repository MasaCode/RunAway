#include "InputManager.h"

namespace MasaEngine{

	InputManager::InputManager() : _mouseCoords(0.0f)
	{
	}


	InputManager::~InputManager()
	{
	}


	void InputManager::update(){
		
		//Loop through _kaymap for each loop, and copy it over to _previouskeymap
		for (auto& itr : _keyMap){
			_previouskeyMap[itr.first] = itr.second;
		}
	}

	void InputManager::presskey(unsigned int keyID){
		_keyMap[keyID] = true;
	}

	void InputManager::releaseKey(unsigned int keyID){
		_keyMap[keyID] = false;
	}

	void InputManager::setMouseCorrds(float x, float y){
		_mouseCoords.x = x;
		_mouseCoords.y = y;
	}

	glm::vec2 InputManager::getMouseCoords() const {
		return _mouseCoords;
	}

	bool InputManager::isKeyDown(unsigned int keyID){
		auto it = _keyMap.find(keyID);
		if (it != _keyMap.end())
			return it->second;
		else
			return false;
	}

	bool InputManager::isKeyPressed(unsigned int keyID){

		if (isKeyDown(keyID) == true && wasKeyDown(keyID) == false){
			return true;
		}

		return false;
	}



	bool InputManager::wasKeyDown(unsigned int keyID){
		auto it = _previouskeyMap.find(keyID);
		if (it != _previouskeyMap.end())
			return it->second;
		else
			return false;
	}

}