#ifndef GL_EXTENSIONS_H
#define GL_EXTENSIONS_H

#include <windows.h>
#include <gl\gl.h>
#include <gl\glext.h>

bool InitMultiTextureARB();
bool InitFrameBufferObjectEXT();
bool InitOcclusionQueriesARB();
bool InitTextureRectangleARB();
bool InitNonPowerOfTwoTexturesARB();

// Multitexture extension
extern PFNGLACTIVETEXTUREARBPROC       glActiveTextureARB;
extern PFNGLMULTITEXCOORD2FARBPROC     glMultiTexCoord2fARB;
extern PFNGLMULTITEXCOORD3FARBPROC     glMultiTexCoord3fARB;
extern PFNGLCLIENTACTIVETEXTUREARBPROC glClientActiveTextureARB;

// Framebuffer object extension
extern PFNGLISRENDERBUFFEREXTPROC						glIsRenderbufferEXT;
extern PFNGLBINDRENDERBUFFEREXTPROC						glBindRenderbufferEXT;
extern PFNGLDELETERENDERBUFFERSEXTPROC					glDeleteRenderbuffersEXT;
extern PFNGLGENRENDERBUFFERSEXTPROC						glGenRenderbuffersEXT;
extern PFNGLRENDERBUFFERSTORAGEEXTPROC					glRenderbufferStorageEXT;
extern PFNGLGETRENDERBUFFERPARAMETERIVEXTPROC			glGetRenderbufferParameterivEXT;
extern PFNGLISFRAMEBUFFEREXTPROC						glIsFramebufferEXT;
extern PFNGLBINDFRAMEBUFFEREXTPROC						glBindFramebufferEXT;
extern PFNGLDELETEFRAMEBUFFERSEXTPROC					glDeleteFramebuffersEXT;
extern PFNGLGENFRAMEBUFFERSEXTPROC						glGenFramebuffersEXT;
extern PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC				glCheckFramebufferStatusEXT;
extern PFNGLFRAMEBUFFERTEXTURE1DEXTPROC					glFramebufferTexture1DEXT;
extern PFNGLFRAMEBUFFERTEXTURE2DEXTPROC					glFramebufferTexture2DEXT;
extern PFNGLFRAMEBUFFERTEXTURE3DEXTPROC					glFramebufferTexture3DEXT;
extern PFNGLFRAMEBUFFERRENDERBUFFEREXTPROC				glFramebufferRenderbufferEXT;
extern PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVEXTPROC	glGetFramebufferAttachmentParameterivEXT;
extern PFNGLGENERATEMIPMAPEXTPROC						glGenerateMipmapEXT;

// Occlusion Queries extension (count the number of drawn pixels)
extern PFNGLGENQUERIESARBPROC			glGenQueries;
extern PFNGLDELETEQUERIESARBPROC		glDeleteQueries;
extern PFNGLBEGINQUERYARBPROC			glBeginQuery;
extern PFNGLENDQUERYARBPROC				glEndQuery;
extern PFNGLGETQUERYIVARBPROC			glGetQueryiv;
extern PFNGLGETQUERYOBJECTIVARBPROC		glGetQueryObjectiv;
extern PFNGLGETQUERYOBJECTUIVARBPROC	glGetQueryObjectuiv;


#endif