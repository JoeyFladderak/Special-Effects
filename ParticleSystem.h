#ifndef _PARTICLE_SYSTEM_H_
#define _PARTICLE_SYSTEM_H_

#include "vector3.h"
#include <vector>

struct Particle
{
	vector3 m_Position;
	vector3 m_Velocity;
	vector3 m_Color;
	float m_Alpha;
	float m_Rotation;
	float m_Size;
	float m_Age;
	float m_Lifetime;
};

class Camera;
class Texture;
class ParticleSystem
{
public:

	ParticleSystem(unsigned int a_NumParticles = 0);
	~ParticleSystem();

	void Update(float a_DT);
	void Render(Camera* a_Cam, int sfactor, int dfactor);

protected:

	std::vector<Particle> m_Particles;

	vector3 m_Position;
	Texture* m_Texture;
};
#endif