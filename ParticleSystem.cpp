#include "ParticleSystem.h"
#include "Texture.h"
#include "mathHelper.h"
#include "f00quat.h"
#include "camera.h"

ParticleSystem::ParticleSystem( unsigned int a_NumParticles /*= 0*/ )
{
	m_Texture = new Texture("Textures/dark_smoke2.png");
	m_Particles.resize(a_NumParticles);
	m_Position = vector3( -19.904f, 0.226f, -0.432f );

	for(unsigned int i = 0; i < a_NumParticles; i++)
	{
		m_Particles[i].m_Age = 0;
		m_Particles[i].m_Lifetime = MathHelper::rFloatRange(2, 3);
		m_Particles[i].m_Rotation = 0;
		m_Particles[i].m_Size = 0;
		m_Particles[i].m_Color = vector3(0, 0, 0);
		m_Particles[i].m_Position = vector3(0, 0, 0);
		m_Particles[i].m_Alpha = 0;
	}
}

ParticleSystem::~ParticleSystem()
{

}

void ParticleSystem::Update( float a_DT )
{
	for(unsigned int i = 0; i < m_Particles.size(); ++i)
	{
		Particle& particle = m_Particles[i];

		particle.m_Age += a_DT;
		if(particle.m_Age > particle.m_Lifetime)
		{
			particle.m_Age = 0.0f;
			particle.m_Lifetime = MathHelper::rFloatRange(2, 3);

			particle.m_Position = vector3(MathHelper::rFloatRange(-0.001, 0.001), MathHelper::rFloatRange(-0.001, 0.001), MathHelper::rFloatRange(-0.001, 0.001));
			particle.m_Velocity = vector3(MathHelper::rFloatRange(-0.01, 0.01), MathHelper::rFloatRange(0, 0.01), MathHelper::rFloatRange(-0.01, 0.01));
		}

		float liferatio = MathHelper::Clamp((particle.m_Age / particle.m_Lifetime), 0, 1);
		particle.m_Velocity += (vector3(0, 0.05, 0) * a_DT);
		particle.m_Position += (particle.m_Velocity * a_DT);
		particle.m_Color = vector3(0.25f, 0.25f, 0.25f);
		particle.m_Rotation = MathHelper::Lerp(0.0f, 20.0f, liferatio);
		particle.m_Size = MathHelper::Lerp(0.01f, 0.05f, liferatio);
		particle.m_Alpha = MathHelper::Lerp(1, 0, liferatio);
	}
}

void ParticleSystem::Render(Camera* a_Cam, int sfactor, int dfactor)
{
	glDepthMask(GL_FALSE);
	glDisable(GL_LIGHTING);
	glEnable(GL_BLEND);
	glBlendFunc(sfactor,dfactor);

	glPushMatrix();
	glTranslatef(m_Position.x, m_Position.y, m_Position.z);

	const vector3 X(0.5f, 0.0f, 0.0f);
	const vector3 Y(0.0f, 0.5f, 0.0f);
	const vector3 Z(0.0f, 0.0f, 1.0f);

	for(unsigned int i = 0; i < m_Particles.size(); ++i)
	{
		Particle& particle = m_Particles[i];
		f00Quat rot(particle.m_Rotation, Z.x, Z.y, Z.z);

		vector3 pos1 = particle.m_Position + (((-X - Y) * particle.m_Size) * rot) * a_Cam->GetOrientation();
		vector3 pos2 = particle.m_Position + (((X - Y) * particle.m_Size) * rot) * a_Cam->GetOrientation();
		vector3 pos3 = particle.m_Position + (((X + Y) * particle.m_Size) * rot) * a_Cam->GetOrientation();
		vector3 pos4 = particle.m_Position + (((-X + Y) * particle.m_Size) * rot) * a_Cam->GetOrientation();
		m_Texture->BindTexture();

		glColor4f(particle.m_Color.x, particle.m_Color.y, particle.m_Color.z, particle.m_Alpha);
		glBegin( GL_QUADS );
		{
			glTexCoord2f( 0.0f, 1.0f );
			glVertex3f( pos1.x, pos1.y, pos1.z );

			glTexCoord2f( 1.0f, 1.0f );
			glVertex3f( pos2.x, pos2.y, pos2.z );

			glTexCoord2f( 1.0f, 0.0f );
			glVertex3f( pos3.x, pos3.y, pos3.z);

			glTexCoord2f( 0.0f, 0.0f );
			glVertex3f( pos4.x, pos4.y, pos4.z );
		}
		glEnd();
		
	}

	glDisable(GL_BLEND);
	glEnable(GL_LIGHTING);
	glDepthMask(GL_TRUE); 
	glPopMatrix();
}
