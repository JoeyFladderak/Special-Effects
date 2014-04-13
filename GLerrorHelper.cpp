#include "GLerrorHelper.h"

#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include "stdio.h"

#ifdef DO_OPENGL_ERRORCHECKING

void CheckOGLerror()
{
	char errorString[0xffff];
	errorString[0] = 0;

	GLenum curError;
	do 
	{
		curError = glGetError();
		if( curError != GL_NO_ERROR )
		{
			char tmps[256];
			sprintf_s( tmps, 256, "'%s' ", gluErrorString(curError) );
			strcat_s( errorString, 0xffff-1, tmps );
		}
	} while ( curError != GL_NO_ERROR );

	if( strlen( errorString) != 0 )
	{
		DebugBreak();
		MessageBox( NULL, errorString, "OpenGL errors", MB_ICONSTOP );
		exit(1);
	}
}

#endif