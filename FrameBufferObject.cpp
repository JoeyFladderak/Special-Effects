#include "FrameBufferObject.h"
#include "GLextensions.h"
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include "GLerrorHelper.h"

FrameBufferObject::FrameBufferObject(void)
	:m_NumRenderTargets(0)
{
}

FrameBufferObject::~FrameBufferObject(void)
{
	for(int i=0;i<m_NumRenderTargets;i++ )
	{
		glDeleteTextures(1, &m_TextureId[i]);
	}

	glDeleteFramebuffersEXT(1, &m_fboId);
	glDeleteTextures(1, &m_DepthId );
}

void FrameBufferObject::Init( int width, int height, int numRenderTargets, GLuint pixelFormat, bool bUseMipmaps, bool bFilter )
{
	m_Width = width;
	m_Height = height;
	m_bUseMipmaps = false;

	m_NumRenderTargets = numRenderTargets;

	// create a framebuffer object, you need to delete them when program exits.
	glGenFramebuffersEXT(1, &m_fboId);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_fboId);
	CheckOGLerror();

	// init the color textures

	for( int i=0;i<numRenderTargets; i++ )
	{
		glGenTextures(1, &(m_TextureId[i]));
		glBindTexture(GL_TEXTURE_2D, m_TextureId[i]);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		
		if( bFilter == false )
		{
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			m_bUseMipmaps = false;
		}
		else
		{
			if( m_bUseMipmaps )
			{
				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			}
			else
			{
				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			}
		}
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE); // automatic mipmap generation included in OpenGL v1.4

		GLenum pixelType = GL_UNSIGNED_BYTE;
		if( pixelFormat == GL_RGBA16F_ARB )
		{
			pixelType = GL_FLOAT;
		}
		else if( pixelFormat == GL_RGBA8 )
		{
			pixelType = GL_UNSIGNED_BYTE;
		}
		else
		{
			assert( 0 );		// only GL_RGBA8 and GL_RGBA16F_ARB supported for now!
		}

		glTexImage2D(GL_TEXTURE_2D, 0, pixelFormat, width, height, 0, GL_RGBA, pixelType, 0);
		glBindTexture(GL_TEXTURE_2D, 0);

		// attach a texture to FBO color attachement point
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT+i, GL_TEXTURE_2D, m_TextureId[i], 0);
		CheckOGLerror();
	}


	// attach a texture for the depth buffer

	glGenTextures(1, &m_DepthId );
	glBindTexture(GL_TEXTURE_2D, m_DepthId );

	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, width, height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_TEXTURE_2D, m_DepthId, 0);




	// check FBO status
	GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
	if(status != GL_FRAMEBUFFER_COMPLETE_EXT)
	{
		char tmps[256];
		GetFrameBufferErrorString( status, tmps, 256 );

		char tmps2[256];
		sprintf_s( tmps2, 256, "FBO not setup OK: '%s'", tmps );

		MessageBox( NULL, tmps2,"Error",MB_ICONSTOP );
	}

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
}


void FrameBufferObject::SetAsCurrentTarget()
{
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_fboId);
}


void FrameBufferObject::BindTexture( int index )
{
	glBindTexture(GL_TEXTURE_2D, m_TextureId[index]);
	CheckOGLerror();
}


void FrameBufferObject::BindDepthBufferTexture()
{
	glBindTexture(GL_TEXTURE_2D, m_DepthId );
	CheckOGLerror();
}


void FrameBufferObject::StopRenderingToMe()
{
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0); // unbind

	if( m_bUseMipmaps )
	{
		for( int i=0;i<m_NumRenderTargets;i++)
		{
			glBindTexture(GL_TEXTURE_2D, m_TextureId[i]);
			glGenerateMipmapEXT(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}
}


void FrameBufferObject::GetFrameBufferErrorString( GLenum status, char* tmps, unsigned int maxString )
{
	switch(status)
	{
	case GL_FRAMEBUFFER_COMPLETE_EXT:
		strcpy_s( tmps, maxString, "Framebuffer complete");
		return;

	case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT:
		strcpy_s( tmps, maxString, "Framebuffer incomplete: Attachment is NOT complete" );
		return;

	case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT:
		strcpy_s( tmps, maxString, "Framebuffer incomplete: No image is attached to FBO");
		return;

	case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
		strcpy_s( tmps, maxString, "Framebuffer incomplete: Attached images have different dimensions" );
		return;

	case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT:
		strcpy_s( tmps, maxString, "Framebuffer incomplete: Color attached images have different internal formats" );
		return;

	case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT:
		strcpy_s( tmps, maxString, "Framebuffer incomplete: Draw buffer" );
		return;

	case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT:
		strcpy_s( tmps, maxString, "Framebuffer incomplete: Read buffer" );
		return;

	case GL_FRAMEBUFFER_UNSUPPORTED_EXT:
		strcpy_s( tmps, maxString, "Unsupported by FBO implementation" );
		return;

	default:
		strcpy_s( tmps, maxString, "Unknow error" );
		return;
	}

}