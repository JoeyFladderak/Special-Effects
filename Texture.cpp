#include "Texture.h"
#include <GdiPlus.h>
#include <GdiPlusBitmap.h>
#include "mathHelper.h"

// this function is required to convert a single-byte character string into a windows unicode string
// I should put this in its own helper class so I can use it in more places than here, but so far the need has not arisen yet...
LPWSTR ConvertLPCSTRToLPWSTR ( const char* pCstring)
{
	LPWSTR pszOut = NULL;

	if (pCstring != NULL)
	{
		int nInputStrLen = strlen (pCstring);

		// Double NULL Termination

		int nOutputStrLen = MultiByteToWideChar(CP_ACP, 0, pCstring, nInputStrLen, NULL, 0) + 2;

		pszOut = new WCHAR [nOutputStrLen];

		if (pszOut)
		{
			memset (pszOut, 0x00, sizeof (WCHAR)*nOutputStrLen);
			MultiByteToWideChar (CP_ACP, 0, pCstring, nInputStrLen, pszOut, nInputStrLen);
		}
	}

	return pszOut;
}


Texture::Texture(void)
	:m_GLtexture(0)
{

}


Texture::Texture( const std::string& filename, GLint minFilter, GLint maxFilter, GLint wrapS, GLint wrapT, bool mipMaps )
	:m_GLtexture(0)
{
	Load( filename, minFilter, maxFilter, wrapS, wrapT, mipMaps );
}


Texture::~Texture(void)
{
	if( m_GLtexture != 0 )
	{
		glDeleteTextures( 1, &m_GLtexture );
	}
}


void Texture::Load( const std::string& filename, GLint minFilter, GLint maxFilter, GLint wrapS, GLint wrapT, bool mipMaps )
{
	if( m_GLtexture != 0 )
	{
		glDeleteTextures( 1, &m_GLtexture );
	}

	m_Filename = filename;

	LPWSTR w_fileName = ConvertLPCSTRToLPWSTR( m_Filename.c_str() );
	Gdiplus::Bitmap image( w_fileName );	// load the file
	delete[] w_fileName;

	if( !MathHelper::IsPowerOf2( image.GetWidth()) || !MathHelper::IsPowerOf2( image.GetHeight() ) )
	{
		MessageBox(NULL, "Texture is not a power of 2!","Error",MB_ICONSTOP );
		exit(1);
	}


	unsigned int* data;						// Stored Data

	// Create storage space for texture data
	data = (unsigned int*)new GLuint[((image.GetWidth() * image.GetHeight())* 4 * sizeof(unsigned int))];

	for( unsigned int x=0; x<image.GetWidth(); x++)
	{
		for( unsigned int y=0; y<image.GetHeight(); y++ )
		{
			Gdiplus::Color color;
			image.GetPixel( x, y, &color );

			unsigned int red = color.GetR();
			unsigned int green = color.GetG();
			unsigned int blue = color.GetB();
			unsigned int alpha = color.GetAlpha();

			data[ y*image.GetWidth() + x] = (red) | (green << 8) | (blue << 16) | (alpha << 24);
		}
	}

	glGenTextures(1, &m_GLtexture);					// Create 1 Texture

	glBindTexture(GL_TEXTURE_2D, m_GLtexture);			// Bind The Texture
	if( mipMaps )
	{
		gluBuild2DMipmaps( GL_TEXTURE_2D, 4, image.GetWidth(), image.GetHeight(), GL_RGBA, GL_UNSIGNED_BYTE, data);			// Build Texture Using Information In data
	}
	else
	{
//		glTexImage2D(GL_TEXTURE_2D, 0, 4, image.GetWidth(), image.GetHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, data);			// Build Texture Using Information In data
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, image.GetWidth(), image.GetHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, data);			// Build Texture Using Information In data
	}
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,minFilter);		// set linear filtering
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,maxFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS );		// make sure texture repeats
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT );

	delete [] data;							// Release data as it has been copied into OpenGL owned memory
}


void Texture::BindTexture()
{
	glBindTexture( GL_TEXTURE_2D, m_GLtexture );
}

void Texture::CreateDepthTexture( int width, int height, GLint internalFormat )
{
	glGenTextures(1, &m_GLtexture );
	glBindTexture(GL_TEXTURE_2D, m_GLtexture );

	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, width, height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}
