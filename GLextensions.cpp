#include "GLextensions.h"

// Multitexture extension
PFNGLACTIVETEXTUREARBPROC       glActiveTextureARB=0;
PFNGLMULTITEXCOORD2FARBPROC     glMultiTexCoord2fARB=0;
PFNGLMULTITEXCOORD3FARBPROC     glMultiTexCoord3fARB=0;
PFNGLCLIENTACTIVETEXTUREARBPROC glClientActiveTextureARB=0;

// Framebuffer object extension
PFNGLISRENDERBUFFEREXTPROC						glIsRenderbufferEXT=0;
PFNGLBINDRENDERBUFFEREXTPROC					glBindRenderbufferEXT=0;
PFNGLDELETERENDERBUFFERSEXTPROC					glDeleteRenderbuffersEXT=0;
PFNGLGENRENDERBUFFERSEXTPROC					glGenRenderbuffersEXT=0;
PFNGLRENDERBUFFERSTORAGEEXTPROC					glRenderbufferStorageEXT=0;
PFNGLGETRENDERBUFFERPARAMETERIVEXTPROC			glGetRenderbufferParameterivEXT=0;
PFNGLISFRAMEBUFFEREXTPROC						glIsFramebufferEXT=0;
PFNGLBINDFRAMEBUFFEREXTPROC						glBindFramebufferEXT=0;
PFNGLDELETEFRAMEBUFFERSEXTPROC					glDeleteFramebuffersEXT=0;
PFNGLGENFRAMEBUFFERSEXTPROC						glGenFramebuffersEXT=0;
PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC				glCheckFramebufferStatusEXT=0;
PFNGLFRAMEBUFFERTEXTURE1DEXTPROC				glFramebufferTexture1DEXT=0;
PFNGLFRAMEBUFFERTEXTURE2DEXTPROC				glFramebufferTexture2DEXT=0;
PFNGLFRAMEBUFFERTEXTURE3DEXTPROC				glFramebufferTexture3DEXT=0;
PFNGLFRAMEBUFFERRENDERBUFFEREXTPROC				glFramebufferRenderbufferEXT=0;
PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVEXTPROC glGetFramebufferAttachmentParameterivEXT=0;
PFNGLGENERATEMIPMAPEXTPROC						glGenerateMipmapEXT=0;

// GenQueriesARB extension (count the number of drawn pixels)
PFNGLGENQUERIESARBPROC			glGenQueries		= NULL;
PFNGLDELETEQUERIESARBPROC		glDeleteQueries		= NULL;
PFNGLBEGINQUERYARBPROC			glBeginQuery		= NULL;
PFNGLENDQUERYARBPROC			glEndQuery			= NULL;
PFNGLGETQUERYIVARBPROC			glGetQueryiv		= NULL;
PFNGLGETQUERYOBJECTIVARBPROC	glGetQueryObjectiv	= NULL;
PFNGLGETQUERYOBJECTUIVARBPROC	glGetQueryObjectuiv	= NULL;


bool InitMultiTextureARB()
{
	char *ext = (char*)glGetString( GL_EXTENSIONS );

	if( strstr( ext, "GL_ARB_multitexture" ) == NULL )
	{
		return false;
	}
	else
	{
		glActiveTextureARB       = (PFNGLCLIENTACTIVETEXTUREARBPROC)wglGetProcAddress("glActiveTextureARB");
		glMultiTexCoord2fARB     = (PFNGLMULTITEXCOORD2FARBPROC)wglGetProcAddress("glMultiTexCoord2fARB");
		glMultiTexCoord3fARB     = (PFNGLMULTITEXCOORD3FARBPROC)wglGetProcAddress("glMultiTexCoord3fARB");
		glClientActiveTextureARB = (PFNGLACTIVETEXTUREARBPROC)wglGetProcAddress("glClientActiveTextureARB");

		if( !glActiveTextureARB || !glMultiTexCoord2fARB || !glMultiTexCoord2fARB || !glClientActiveTextureARB )
		{
			return false;
		}
	}

	return true;
}


bool InitFrameBufferObjectEXT()
{
	char *ext = (char*)glGetString( GL_EXTENSIONS );

	if( strstr( ext, "GL_EXT_framebuffer_object" ) == NULL )
	{
		return false;
	}
	else
	{
		glIsRenderbufferEXT								= (PFNGLISRENDERBUFFEREXTPROC)wglGetProcAddress("glIsRenderbufferEXT");
		glBindRenderbufferEXT							= (PFNGLBINDRENDERBUFFEREXTPROC)wglGetProcAddress("glBindRenderbufferEXT");
		glDeleteRenderbuffersEXT						= (PFNGLDELETERENDERBUFFERSEXTPROC)wglGetProcAddress("glDeleteRenderbuffersEXT");
		glGenRenderbuffersEXT							= (PFNGLGENRENDERBUFFERSEXTPROC)wglGetProcAddress("glGenRenderbuffersEXT");
		glRenderbufferStorageEXT						= (PFNGLRENDERBUFFERSTORAGEEXTPROC)wglGetProcAddress("glRenderbufferStorageEXT");
		glGetRenderbufferParameterivEXT					= (PFNGLGETRENDERBUFFERPARAMETERIVEXTPROC)wglGetProcAddress("glGetRenderbufferParameterivEXT");
		glIsFramebufferEXT								= (PFNGLISFRAMEBUFFEREXTPROC)wglGetProcAddress("glIsFramebufferEXT");
		glBindFramebufferEXT							= (PFNGLBINDFRAMEBUFFEREXTPROC)wglGetProcAddress("glBindFramebufferEXT");
		glDeleteFramebuffersEXT							= (PFNGLDELETEFRAMEBUFFERSEXTPROC)wglGetProcAddress("glDeleteFramebuffersEXT");
		glGenFramebuffersEXT							= (PFNGLGENFRAMEBUFFERSEXTPROC)wglGetProcAddress("glGenFramebuffersEXT");
		glCheckFramebufferStatusEXT						= (PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC)wglGetProcAddress("glCheckFramebufferStatusEXT");
		glFramebufferTexture1DEXT						= (PFNGLFRAMEBUFFERTEXTURE1DEXTPROC)wglGetProcAddress("glFramebufferTexture1DEXT");
		glFramebufferTexture2DEXT						= (PFNGLFRAMEBUFFERTEXTURE2DEXTPROC)wglGetProcAddress("glFramebufferTexture2DEXT");
		glFramebufferTexture3DEXT						= (PFNGLFRAMEBUFFERTEXTURE3DEXTPROC)wglGetProcAddress("glFramebufferTexture3DEXT");
		glFramebufferRenderbufferEXT					= (PFNGLFRAMEBUFFERRENDERBUFFEREXTPROC)wglGetProcAddress("glFramebufferRenderbufferEXT");
		glGetFramebufferAttachmentParameterivEXT		= (PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVEXTPROC)wglGetProcAddress("glGetFramebufferAttachmentParameterivEXT");
		glGenerateMipmapEXT								= (PFNGLGENERATEMIPMAPEXTPROC)wglGetProcAddress("glGenerateMipmapEXT");

		if(	   !glIsRenderbufferEXT 
			|| !glBindRenderbufferEXT 
			|| !glDeleteRenderbuffersEXT 
			|| !glGenRenderbuffersEXT
			|| !glRenderbufferStorageEXT					
			|| !glGetRenderbufferParameterivEXT				
			|| !glIsFramebufferEXT							
			|| !glBindFramebufferEXT						
			|| !glDeleteFramebuffersEXT						
			|| !glGenFramebuffersEXT						
			|| !glCheckFramebufferStatusEXT					
			|| !glFramebufferTexture1DEXT					
			|| !glFramebufferTexture2DEXT					
			|| !glFramebufferTexture3DEXT					
			|| !glFramebufferRenderbufferEXT				
			|| !glGetFramebufferAttachmentParameterivEXT	
			|| !glGenerateMipmapEXT	)
		{
			return false;
		}

	}


	return true;
}


bool InitOcclusionQueriesARB()
{
	char *ext = (char*)glGetString( GL_EXTENSIONS );

	if( strstr( ext, "GL_ARB_occlusion_query" ) == NULL )
	{
		return false;
	}
	else
	{
		glGenQueries		= (PFNGLGENQUERIESARBPROC)wglGetProcAddress("glGenQueriesARB");
		glDeleteQueries		= (PFNGLDELETEQUERIESARBPROC)wglGetProcAddress("glDeleteQueriesARB");
		glBeginQuery		= (PFNGLBEGINQUERYARBPROC)wglGetProcAddress("glBeginQueryARB");
		glEndQuery			= (PFNGLENDQUERYARBPROC)wglGetProcAddress("glEndQueryARB");
		glGetQueryiv		= (PFNGLGETQUERYIVARBPROC)wglGetProcAddress("glGetQueryivARB");
		glGetQueryObjectiv	= (PFNGLGETQUERYOBJECTIVARBPROC)wglGetProcAddress("glGetQueryObjectivARB");
		glGetQueryObjectuiv = (PFNGLGETQUERYOBJECTUIVARBPROC)wglGetProcAddress("glGetQueryObjectuivARB");

		if( !glGenQueries || !glDeleteQueries || !glBeginQuery || !glEndQuery || !glGetQueryiv || !glGetQueryObjectiv || !glGetQueryObjectuiv )
		{
			return false;
		}
	}


	GLint bitsSupported;

	// check to make sure functionality is supported
	glGetQueryiv( GL_SAMPLES_PASSED_ARB, GL_QUERY_COUNTER_BITS_ARB, &bitsSupported );
	if (bitsSupported == 0) 
	{	
			return false;	// we need this specifically!
	}

	return true;
}


bool InitTextureRectangleARB()
{
	char *ext = (char*)glGetString( GL_EXTENSIONS );

	if( strstr( ext, "GL_ARB_texture_rectangle" ) == NULL )
	{
		return false;
	}
	else
	{
		return true;
	}
}


bool InitNonPowerOfTwoTexturesARB()
{
	char *ext = (char*)glGetString( GL_EXTENSIONS );

	if( strstr( ext, "GL_ARB_texture_non_power_of_two" ) == NULL )
	{
		return false;
	}
	else
	{
		return true;
	}
}
