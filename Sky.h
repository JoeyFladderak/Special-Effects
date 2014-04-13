#ifndef _SKY_H_
#define _SKY_H_

#include <Cg/cg.h>
#include <Cg/cgGL.h>
#include "Shader.h"
#include "OpenMDLmodel.h"
#include "Texture.h"
#include "vector3.h"

class Sky
{
public:

	Sky();
	~Sky();

	void Init(CGcontext a_Context, const char* a_Shader);
	void Enable();
	void Disable();

	void UpdateParameters();

	void DayNightCycle(vector3& a_LightPos);
	void RenderSky(vector3& a_CamPos);

	float& GetScrollSpeed() {return m_ScrollSpeed;}
	float& GetCycleSpeed() {return m_CycleSpeed;}
	float& GetMoonFullness() {return m_MoonFullness;}
	float& GetAtmosphere() {return m_TweakAtmos;}
	float& GetLightStrength() {return m_TweakLightStrength;}
	bool& GetRenderClouds() {return m_EnableClouds;}
	bool& GetRenderStars() {return m_EnableStars;}
	bool& GetCycle() {return m_Cycle;}


protected:

	OpenMDLmodel m_SkySphere;
	Shader m_SkyShader;
	CGparameter m_SkyModelCamProj;
	CGparameter m_Sunpos;
	CGparameter m_Moonpos;
	CGparameter m_Moonsunangle;
	CGparameter m_AtmosThicknes;
	CGparameter m_SkyScroll;
	CGparameter m_LightStrength;
	CGparameter m_clouds;
	CGparameter m_stars;
	Texture m_Gradient1;
	Texture m_Gradient2;
	Texture m_Sun;
	Texture m_SunGradient;
	Texture m_Moon;
	Texture m_moonmask;
	Texture m_moonhalo;
	Texture m_starfield;
	Texture m_cloudfield;

	vector3 m_LightPos;

	float m_ScrollSpeed;
	float m_CycleSpeed;
	float m_MoonFullness;

	bool m_EnableStars;
	bool m_EnableClouds;
	bool m_Cycle;

	float m_TweakAtmos;
	float m_TweakLightStrength;

};
#endif