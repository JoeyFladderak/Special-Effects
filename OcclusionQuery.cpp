#include "OcclusionQuery.h"
#include "GLextensions.h"

OcclusionQuery::OcclusionQuery()
{
	m_Query = 0;
	m_HasRunAtLeastOnce = false;

	glGenQueries(1, &m_Query);
}

OcclusionQuery::~OcclusionQuery()
{

}

void OcclusionQuery::Start()
{
	m_HasRunAtLeastOnce = true;
	glBeginQuery(GL_SAMPLES_PASSED_ARB, m_Query);
}

void OcclusionQuery::End()
{
	glEndQuery(GL_SAMPLES_PASSED_ARB);
}

unsigned int OcclusionQuery::GetNumPixels()
{
	if(!m_HasRunAtLeastOnce)
		return 0;

	unsigned int pixelsdrawn;
	glGetQueryObjectuiv(m_Query, GL_QUERY_RESULT_ARB, &pixelsdrawn);

	return pixelsdrawn;
}
