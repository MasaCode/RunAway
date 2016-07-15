#include "ParticleBatch2D.h"

namespace MasaEngine{
///////////////////////////////////////////////////// ParticleBatch2D class /////////////////////////////////////////////////////

	ParticleBatch2D::ParticleBatch2D()
	{
		//Empty
	}


	ParticleBatch2D::~ParticleBatch2D()
	{
		delete[] _particles;
	}


	void ParticleBatch2D::init(int maxParticles, float decayRate, GLTexture texture, std::function<void(Particle2D&, float)> updateFunc){
		_maxParticles = maxParticles;
		_decayRate = decayRate;
		_texture = texture;
		_particles = new Particle2D[_maxParticles];

		_updateFunc = updateFunc;

	}

	void ParticleBatch2D::update(float deltaTime){
		for (int i = 0; i < _maxParticles; i++){
			//Check if it is active
			if (_particles[i].lifeTime > 0.0f){
				_updateFunc(_particles[i], deltaTime);
				_particles[i].lifeTime -= _decayRate * deltaTime;
			}
		}
	}

	void ParticleBatch2D::draw(SpriteBatch* spriteBatch){
		glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
		for (int i = 0; i < _maxParticles; i++){
			auto& p = _particles[i];
			if (p.lifeTime > 0.0f){

				glm::vec4 destRect(p.position.x,p.position.y, p.size, p.size);
				spriteBatch->draw(destRect,uvRect,_texture.id,0.0f,p.color);
			}
		}
	}

	void ParticleBatch2D::addParticle(const glm::vec2& position, const glm::vec2& velocity, const Color& color, float size){
		int particleIndex = findFreeParticle();

		auto& p = _particles[particleIndex];
		p.lifeTime = 1.0f;
		p.position = position;
		p.velocity = velocity;
		p.color = color;
		p.size = size;
	}

	int ParticleBatch2D::findFreeParticle(){
		for (int i = _lastFreeParticle; i < _maxParticles; i++){
			if (_particles[i].lifeTime <= 0.0f){
				_lastFreeParticle = i;
				return i;
			}
		}

		for (int i = 0; i < _lastFreeParticle; i++){
			if (_particles[i].lifeTime <= 0.0f){
				_lastFreeParticle = i;
				return i;
			}
		}

		return 0; //error
	}

}