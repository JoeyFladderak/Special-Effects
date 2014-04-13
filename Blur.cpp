#include "Blur.h"
#include "mathHelper.h"
#include <stdio.h>

Blur::Blur()
{

}

Blur::~Blur()
{

}

void Blur::Init( CGcontext a_Context, const char* a_Shader )
{
	m_HorizontalBlur.Init(a_Context, "Shaders/BlurHor");
	m_VerticalBlur.Init(a_Context, "Shaders/BlurVert");
	char buffer[256];
	for(int i = 0; i < 32; i++)
	{
		sprintf(buffer, "weights[%d]", i);
		m_HorizontalWeights[i] = cgGetNamedParameter(m_HorizontalBlur.GetPixelProgram(), buffer);
		m_VerticalWeights[i] = cgGetNamedParameter(m_VerticalBlur.GetPixelProgram(), buffer);

		m_Weights[i] = MathHelper::rFloatRange(0.0f, 0.1f);
	}
}

void Blur::Enable()
{
	for(int i = 0; i < 32; i++)
	{
	 	cgGLSetParameter1f(m_HorizontalWeights[i], m_Weights[i]);
	 	cgGLSetParameter1f(m_VerticalWeights[i], m_Weights[i]);
	}

	m_HorizontalBlur.EnablePrograms();
	m_VerticalBlur.EnablePrograms();
}

void Blur::Disable()
{
	m_VerticalBlur.DisablePrograms();
	m_HorizontalBlur.DisablePrograms();
}
