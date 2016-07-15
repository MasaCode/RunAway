#pragma once

#include <unordered_map>
#include <glm\glm.hpp>

namespace MasaEngine{

	class InputManager
	{
	public:
		InputManager();
		~InputManager();

		void update();

		void presskey(unsigned int keyID);
		void releaseKey(unsigned int keyID);

		// Returns true if the key is held down
		bool isKeyDown(unsigned int keyID);
		
		// Returns true if the key was just pressed
		bool isKeyPressed(unsigned int keyID);

		void setMouseCorrds(float x, float y);

		//getters
		glm::vec2 getMouseCoords() const ;


	private:
		bool wasKeyDown(unsigned int keyID);

	private:
		std::unordered_map<unsigned int, bool> _keyMap;
		std::unordered_map<unsigned int, bool> _previouskeyMap;
		glm::vec2 _mouseCoords;
	};

}