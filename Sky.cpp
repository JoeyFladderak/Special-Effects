#include "Sky.h"
#include "matrix.h"
#include "GLextensions.h"

Sky::Sky()
{
	
}

Sky::~Sky()
{

}

void Sky::Init( CGcontext a_Context, const char* a_Shader )
{
	m_SkyShader.Init(a_Context, "Shaders/Sky");
	m_SkyModelCamProj = cgGetNamedParameter(m_SkyShader.GetVertexProgram(), "ModelCamProj");
	m_Sunpos = cgGetNamedParameter(m_SkyShader.GetVertexProgram(), "sunposnormalized");
	m_Moonpos = cgGetNamedParameter(m_SkyShader.GetVertexProgram(), "moonposnormalized");
	m_Moonsunangle = cgGetNamedParameter(m_SkyShader.GetPixelProgram(), "moonsunangle");
	m_AtmosThicknes = cgGetNamedParameter(m_SkyShader.GetPixelProgram(), "atmosthickness");
	m_SkyScroll = cgGetNamedParameter(m_SkyShader.GetPixelProgram(), "skyscroll");
	m_LightStrength = cgGetNamedParameter(m_SkyShader.GetPixelProgram(), "lightstrength");
	m_clouds = cgGetNamedParameter(m_SkyShader.GetPixelProgram(), "clouds");
	m_stars = cgGetNamedParameter(m_SkyShader.GetPixelProgram(), "stars");

	m_Gradient1.Load("Textures/skygradient_blue.png");
	m_Gradient2.Load("Textures/skygradient_dark.png");
	m_Sun.Load("Textures/sun.png");
	m_SunGradient.Load("Textures/sungradient.png");
	m_Moon.Load("Textures/moon.png");
	m_moonmask.Load("Textures/moonmask.png");
	m_moonhalo.Load("Textures/moonhalo.png");
	m_starfield.Load("Textures/starfield.png");
	m_cloudfield.Load("Textures/clouds.png");

	m_SkySphere.LoadFromFile(a_Context, "models/dome.omd", 1);
	m_ScrollSpeed = 3.0f;
	m_CycleSpeed = 0.01f;
	m_MoonFullness = 0.5f;
	m_EnableClouds = true;
	m_EnableStars = true;
	m_TweakAtmos = 0.9f;
	m_TweakLightStrength = 1.0f;
	m_Cycle = true;
}

void Sky::Enable()
{
	m_SkyShader.EnablePrograms();
}

void Sky::Disable()
{
	m_SkyShader.DisablePrograms();
}

void Sky::UpdateParameters()
{
	cgGLSetStateMatrixParameter( m_SkyModelCamProj, CG_GL_MODELVIEW_PROJECTION_MATRIX, CG_GL_MATRIX_IDENTITY );

	vector3 light = m_LightPos;
	light.Normalize();
	
	cgGLSetParameter3f(m_Sunpos, light.x, light.y, light.z);
	cgGLSetParameter3f(m_Moonpos, -light.x, -light.y, -light.z);
	
	glActiveTextureARB(GL_TEXTURE0_ARB);
	m_Gradient1.BindTexture();
	
	glActiveTextureARB(GL_TEXTURE1_ARB);
	m_Gradient2.BindTexture();
	
	glActiveTextureARB(GL_TEXTURE2_ARB);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	m_Sun.BindTexture();
	
	glActiveTextureARB(GL_TEXTURE3_ARB);
	m_SunGradient.BindTexture();
	
	glActiveTextureARB(GL_TEXTURE4_ARB);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	m_Moon.BindTexture();
	
	glActiveTextureARB(GL_TEXTURE5_ARB);
	m_moonmask.BindTexture();
	
	glActiveTextureARB(GL_TEXTURE6_ARB);
	m_moonhalo.BindTexture();
	
	glActiveTextureARB(GL_TEXTURE7_ARB);
	m_starfield.BindTexture();
	
	glActiveTextureARB(GL_TEXTURE8_ARB);
	m_cloudfield.BindTexture();
	
	static float scroll = 0.0f;
	scroll += m_ScrollSpeed / 100000;
	
	cgGLSetParameter1f(m_Moonsunangle, m_MoonFullness);
	cgGLSetParameter1f(m_AtmosThicknes, m_TweakAtmos);
	cgGLSetParameter1f(m_SkyScroll, scroll);
	cgGLSetParameter1f(m_LightStrength, m_TweakLightStrength);
	cgGLSetParameter1f(m_clouds, m_EnableClouds);
	cgGLSetParameter1f(m_stars, m_EnableStars);
}

void Sky::DayNightCycle( vector3& a_LightPos )
{
//  	static float angleHor = -20;
//  	vector3 orientation;
 
 	m_LightPos = a_LightPos;
 
//  	angleHor -= m_CycleSpeed;
//  
//  	if(angleHor >= 360)
//  	{
//  		angleHor -= 360;
//  		m_MoonFullness += 0.1;
//  
//  		if(m_MoonFullness >= 1.0f)
//  			m_MoonFullness = 0.0f;
//  	}
//  
//  	orientation.z = 0;
//  	orientation.y = 0;
//  	orientation.x = angleHor;
//  
//  	matrix m;
//  	m.Rotate(vector3(0, 0, 0), orientation.x, orientation.y, orientation.z);
//  	a_LightPos = 900 * vector3(m.cell[0], m.cell[4], m.cell[8]);
//  	m.SetTranslation(a_LightPos);
}

void Sky::RenderSky(vector3& a_CamPos)
{
	glPushMatrix();

	vector3 campos = a_CamPos;
	m_SkySphere.SetPosition(campos);

	m_SkySphere.SetTransform();

	UpdateParameters();

	Enable();
	glDepthMask( GL_FALSE );
	m_SkySphere.RenderWithoutDepth();
	glDepthMask( GL_TRUE );
	Disable();
	glPopMatrix();
	glBindTexture(GL_TEXTURE_2D, 0);
}
