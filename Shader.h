#pragma once

#include <Cg/cg.h>
#include <Cg/cgGL.h>

class Shader
{
public:
	Shader(void);
	~Shader(void);

	void Init( CGcontext cgContext, const char* name );
	static void CheckCGError( CGcontext cgContext, const char* a_Situation );

	void EnablePrograms();
	void DisablePrograms();

	CGprogram GetVertexProgram() { return m_Vertex; }
	CGprogram GetPixelProgram() { return m_Pixel; }

protected:
	CGcontext m_cgcontext;
	CGprofile m_Vertexprofile;
	CGprofile m_Pixelprofile;
	CGprogram m_Vertex;
	CGprogram m_Pixel;

	char m_name[256];
};
