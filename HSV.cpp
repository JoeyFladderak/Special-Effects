#include "HSV.h"

HSV::HSV()
{

}

HSV::~HSV()
{

}

void HSV::Init( CGcontext a_CGcontext, const char* a_Shader )
{
	m_HSV.Init(a_CGcontext, a_Shader);
	m_HSV_H =  cgGetNamedParameter( m_HSV.GetPixelProgram(), "H");
	m_HSV_S =  cgGetNamedParameter( m_HSV.GetPixelProgram(), "S");
	m_HSV_V =  cgGetNamedParameter( m_HSV.GetPixelProgram(), "V");

	m_Hue = 0.0f;
	m_Saturation = 0.16;
	m_Value = 0.0072;
}

void HSV::Enable()
{
	m_HSV.EnablePrograms();
}

void HSV::Disable()
{
	m_HSV.DisablePrograms();
}

void HSV::UpdateParameters()
{
	cgGLSetParameter1f( m_HSV_H, m_Hue );
	cgGLSetParameter1f( m_HSV_S, m_Saturation );
	cgGLSetParameter1f( m_HSV_V, m_Value );
}
