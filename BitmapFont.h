#pragma once

#include <windows.h>
#include <gl/gl.h>

// (loosely) based on code found here:
// http://nehe.gamedev.net/tutorial/bitmap_fonts/17002/

class BitmapFont
{
public:
	BitmapFont(void);
	virtual ~BitmapFont(void);

	void BuildFont( char* name, int height, HDC hDC );
	void Print(float x, float y, const char *fmt, ...);	// use 	glRasterPos2f() to set coordinates for printing

protected:
	GLuint	base;				// Base Display List For The Font Set
};
