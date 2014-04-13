#include "BitmapFont.h"
#include <stdio.h>

BitmapFont::BitmapFont(void)
{
}

BitmapFont::~BitmapFont(void)
{
	glDeleteLists(base, 96);							// Delete All 96 Characters
}


void BitmapFont::BuildFont( char* name, int height, HDC hDC )
{
	HFONT	font;										// Windows Font ID
	HFONT	oldfont;									// Used For Good House Keeping

	base = glGenLists(96);								// Storage For 96 Characters

	font = CreateFont(	-height,							// Height Of Font
		0,								// Width Of Font
		0,								// Angle Of Escapement
		0,								// Orientation Angle
		FW_BOLD,						// Font Weight
		FALSE,							// Italic
		FALSE,							// Underline
		FALSE,							// Strikeout
		ANSI_CHARSET,					// Character Set Identifier
		OUT_TT_PRECIS,					// Output Precision
		CLIP_DEFAULT_PRECIS,			// Clipping Precision
		ANTIALIASED_QUALITY,			// Output Quality
		FF_DONTCARE|DEFAULT_PITCH,		// Family And Pitch
//		"Courier New");					// Font Name
		name );							// Font Name

	oldfont = (HFONT)SelectObject(hDC, font);           // Selects The Font We Want
	wglUseFontBitmaps(hDC, 32, 96, base);				// Builds 96 Characters Starting At Character 32
	SelectObject(hDC, oldfont);							// Selects The Font We Want
	DeleteObject(font);									// Delete The Font

}


void BitmapFont::Print(float x, float y, const char *fmt, ...)
{
	char		text[256];								// Holds Our String
	va_list		ap;										// Pointer To List Of Arguments

	if (fmt == NULL)									// If There's No Text
		return;											// Do Nothing

	va_start(ap, fmt);									// Parses The String For Variables
	vsprintf_s(text, 256, fmt, ap);							// And Converts Symbols To Actual Numbers
	va_end(ap);											// Results Are Stored In Text

	glColor3f(1.0,1.0,1.0);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	GLint vPort[4];
	glGetIntegerv(GL_VIEWPORT, vPort);
	glOrtho(0, vPort[2], 0, vPort[3], -1, 1);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glPushAttrib(GL_ALL_ATTRIB_BITS);

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_TEXTURE_2D);

	glRasterPos2f(x, y);

	glEnable(GL_COLOR_MATERIAL);						// Enable Coloring Of Material
	glColorMaterial(GL_FRONT,GL_AMBIENT);		// make sure there is at least a default material present
	glColor3f(1.0,1.0,1.0);

	glColorMaterial(GL_FRONT,GL_SPECULAR);
	glColor3f(1.0,1.0,1.0);

	glColorMaterial(GL_FRONT,GL_DIFFUSE);
	glColor3f(1.0,1.0,1.0);

	glListBase(base - 32);							// Sets The Base Character to 32
	glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);	// Draws The Display List Text
	glPopAttrib();										// Pops The Display List Bits

	glDisable(GL_COLOR_MATERIAL);

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}
