#ifndef _OCCLUSIONQUERY_H_
#define _OCCLUSIONQUERY_H_

#include <Windows.h>
#include <gl/GL.h>

class OcclusionQuery
{
public:

	OcclusionQuery();
	~OcclusionQuery();
	void Start();
	void End();
	unsigned int GetNumPixels();

protected:

	GLuint m_Query;
	bool m_HasRunAtLeastOnce;
};
#endif