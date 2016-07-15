#pragma once

#include <functional>
#include <glm/glm.hpp>
#include "Vertex.h"
#include "SpriteBatch.h"
#include "GLTexture.h"


namespace MasaEngine{

	class Particle2D{
		public:
			glm::vec2 position = glm::vec2(0.0f);
			glm::vec2 velocity = glm::vec2(0.0f);
			Color color;
			float lifeTime = 0.0f;
			float size = 0.0f;
			bool isActive = false;
	};

	inline void defaultParticleUpdate(Particle2D& particle, float deltaTime){
		particle.position += particle.velocity  * deltaTime;

	}



	class ParticleBatch2D
	{
	public:
		ParticleBatch2D();
		~ParticleBatch2D();

		void init(int maxParticles, float decayRate,GLTexture texture,std::function<void(Particle2D&, float)> updateFunc = defaultParticleUpdate);

		void update(float deltaTime);

		void draw(SpriteBatch* spriteBatch);


		void addParticle(const glm::vec2& position ,const glm::vec2& velocity, const Color& color,float size);

	private:
		int findFreeParticle();

		std::function<void(Particle2D&, float)> _updateFunc;

	private:
		float _decayRate = 0.1f;
		Particle2D* _particles = nullptr;
		int _maxParticles = 0;
		int _lastFreeParticle = 0;
		GLTexture _texture;
	};

}