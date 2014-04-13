#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "Shader.h"

Shader::Shader(void)
{
	m_name[0] = 0;
}

Shader::~Shader(void)
{
	cgDestroyProgram( m_Vertex );
	cgDestroyProgram( m_Pixel );
}

void Shader::Init( CGcontext cgContext, const char* name )
{
	strcpy_s( m_name, 256, name );
	assert( strlen( m_name ) != 0 );

	m_cgcontext = cgContext;

	char vertexFileName[256];
	strcpy_s( vertexFileName, 256, m_name );
	strcat_s( vertexFileName, 256, "Vertex.cg" );

	m_Vertexprofile = cgGLGetLatestProfile( CG_GL_VERTEX );
	cgGLSetOptimalOptions( m_Vertexprofile );
	m_Vertex = cgCreateProgramFromFile(
		m_cgcontext, 
		CG_SOURCE,							// Program in human-readable form
		vertexFileName,						// Name of file containing program
		m_Vertexprofile,									// Profile: OpenGL ARB vertex program
		"vertexfunction", NULL );						// Entry function name
	CheckCGError( m_cgcontext, "compiling vertex shader" );
	cgGLLoadProgram( m_Vertex );

	char pixelFileName[256];
	strcpy_s( pixelFileName, 256, m_name );
	strcat_s( pixelFileName, 256, "Pixel.cg" );

	m_Pixelprofile = cgGLGetLatestProfile( CG_GL_FRAGMENT );
	cgGLSetOptimalOptions( m_Pixelprofile );
	m_Pixel = cgCreateProgramFromFile(
		m_cgcontext,										// Cg runtime context
		CG_SOURCE,										// Program in human-readable form
		pixelFileName,						// Name of file containing program
		m_Pixelprofile,									// Profile: OpenGL ARB fragment program
		"pixelfunction", NULL );						// Entry function name
	CheckCGError( m_cgcontext, "compiling pixel shader" );
	cgGLLoadProgram( m_Pixel );

}

void Shader::CheckCGError( CGcontext cgContext, const char* a_Situation )
{
	CGerror error;
	const char* string = cgGetLastErrorString( &error );
	if (error == CG_NO_ERROR) return;
	char t[0xFFFF];
	if (error == CG_COMPILER_ERROR)
	{
		sprintf_s( t, 0xFFFF, "%s\n", cgGetLastListing( cgContext ) );
		MessageBox( NULL, t, "COMPILER ERROR", MB_OK | MB_ICONINFORMATION );
	}
	else
	{
		sprintf_s( t, 0xFFFF, "%s: %s\n", a_Situation, string );
		MessageBox( NULL, t, "CG ERROR", MB_OK | MB_ICONINFORMATION );
	}	
	exit( 1 );
}

void Shader::EnablePrograms()
{
	cgGLBindProgram( m_Vertex );
	cgGLEnableProfile( m_Vertexprofile );
	cgGLBindProgram( m_Pixel );
	cgGLEnableProfile( m_Pixelprofile );
}

void Shader::DisablePrograms()
{
	cgGLDisableProfile( m_Vertexprofile );
	cgGLDisableProfile( m_Pixelprofile );
}