#include "ParticleEngine2D.h"
#include "ParticleBatch2D.h"
#include "SpriteBatch.h"

namespace MasaEngine{

	ParticleEngine2D::ParticleEngine2D()
	{
		//Empty
	}


	ParticleEngine2D::~ParticleEngine2D()
	{
		for (auto& itr : _batches){
			delete itr;
		}

		_batches.clear();
	}

	void ParticleEngine2D::addParticleBatch(ParticleBatch2D* particlebatch){
		_batches.push_back(particlebatch);
	}

	void ParticleEngine2D::update(float deltaTime){
		for (auto& itr : _batches){
			itr->update(deltaTime);
		}
	}

	void ParticleEngine2D::draw(SpriteBatch* spritebatch){
		for (auto& itr : _batches){
			spritebatch->begin();
			
			itr->draw(spritebatch); //It might couse ou a 

			spritebatch->end();
			spritebatch->renderBatch();
		}
	}

}