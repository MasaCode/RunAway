#pragma once
#include <vector>

namespace MasaEngine{

	class ParticleBatch2D;
	class SpriteBatch;

	class ParticleEngine2D
	{
	public:
		ParticleEngine2D();
		~ParticleEngine2D();

		void addParticleBatch(ParticleBatch2D* particlebatch);
		
		void update(float deltaTime);
		
		void draw(SpriteBatch* spritebatch);

	private:
		std::vector<ParticleBatch2D*> _batches;
	};

}

