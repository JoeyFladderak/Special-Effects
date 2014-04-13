#pragma once

#include <windows.h>
#include <GL/GL.h>
#include <gl/glext.h>

class FrameBufferObject
{
public:
	FrameBufferObject(void);
	virtual ~FrameBufferObject(void);

	void Init( int width, int height, int numRenderTargets, GLuint pixelFormat=GL_RGBA8, bool bUseMipmaps=false, bool bFilter=true );
	void SetAsCurrentTarget();
	void StopRenderingToMe();
	void BindTexture( int index );
	void BindDepthBufferTexture();
	unsigned int GetWidth() { return m_Width; }
	unsigned int GetHeight() { return m_Height; }

protected:
	void GetFrameBufferErrorString( GLenum status, char* tmps, unsigned int maxString );

	GLuint m_fboId;                       // ID of FBO
	GLuint m_TextureId[4];               // ID of textures
	int	m_NumRenderTargets;
	GLuint m_DepthId;                     // ID of DepthBuffer texture

	unsigned int m_Width;
	unsigned int m_Height;

	bool m_bUseMipmaps;
};

