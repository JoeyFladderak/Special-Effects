#ifndef GL_ERRORHELPER_H
#define GL_ERRORHELPER_H

#define DO_OPENGL_ERRORCHECKING

#ifdef DO_OPENGL_ERRORCHECKING
extern void CheckOGLerror();
#else
#define CheckOGLerror()
#endif

#endif