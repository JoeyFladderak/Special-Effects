#include "DepthOfField.h"
#include "mathHelper.h"
#include <stdio.h>

DepthOfField::DepthOfField()
{

}

DepthOfField::~DepthOfField()
{

}

void DepthOfField::Init( CGcontext a_Context, const char* a_Shader )
{
	m_DoF.Init(a_Context, a_Shader);

	char buffer[256];
	for(int i = 0; i < DOFSAMPLES; i++)
	{
		sprintf(buffer, "samples[%d]", i);
		m_CGSamples[i] = cgGetNamedParameter(m_DoF.GetPixelProgram(), buffer);
		m_Samples[i].x = MathHelper::rFloatRange(-0.002f, 0.002f);
		m_Samples[i].y = MathHelper::rFloatRange(-0.002f, 0.002f);
	}

	m_BlurSize = cgGetNamedParameter(m_DoF.GetPixelProgram(), "blursize");
	m_MinRange = cgGetNamedParameter(m_DoF.GetPixelProgram(), "minrange");
	m_MaxRange = cgGetNamedParameter(m_DoF.GetPixelProgram(), "maxrange");

	m_TweakMinRange = 1.0f;
	m_TweakMaxRange = 2.0f;
	m_TweakBlurSize = 3.0f;
}

void DepthOfField::Enable()
{
	m_DoF.EnablePrograms();
}

void DepthOfField::Disable()
{
	m_DoF.DisablePrograms();
}

void DepthOfField::UpdateParameters()
{
	for(int i = 0; i < DOFSAMPLES; i++)
	{
		cgGLSetParameter2f(m_CGSamples[i], m_Samples[i].x, m_Samples[i].y);
	}

	cgGLSetParameter1f(m_BlurSize, m_TweakBlurSize);
	cgGLSetParameter1f(m_MinRange, m_TweakMinRange);
	cgGLSetParameter1f(m_MaxRange, m_TweakMaxRange);
}
