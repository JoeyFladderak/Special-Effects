#pragma once

#include <windows.h>
#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <gl\glu.h>			// Header File For The OpenGL32 Library
#include <gl\glext.h>
#include <vector>

class Texture
{
public:
	Texture(void);
	Texture( const std::string& filename, GLint minFilter = GL_LINEAR_MIPMAP_NEAREST, GLint maxFilter = GL_LINEAR, GLint wrapS = GL_REPEAT, GLint wrapT = GL_REPEAT, bool mipMaps = true );

	virtual ~Texture(void);
	GLuint GetOpenGLHandle() { return m_GLtexture; }
	void Load( const std::string& filename, GLint minFilter = GL_LINEAR_MIPMAP_NEAREST, GLint maxFilter = GL_LINEAR, GLint wrapS = GL_REPEAT, GLint wrapT = GL_REPEAT, bool mipMaps = true );
	void BindTexture();
	void CreateDepthTexture( int width, int height, GLint internalFormat=GL_DEPTH_COMPONENT24 );

protected:
	std::string m_Filename;
	GLuint m_GLtexture;
};
