#include <stdlib.h>
#include "TriList.h"

TriList::TriList(void)
{
	m_v0 = NULL;
	m_v1 = NULL;
	m_v2 = NULL;
	m_UV0 = NULL;
	m_UV1 = NULL;
	m_UV2 = NULL;
	m_NumTris = 0;
}

TriList::~TriList(void)
{
	FreeTris();
}

void TriList::AllocateTris( unsigned int numTris )
{
	FreeTris();

	m_v0 = new unsigned int[numTris];
	m_v1 = new unsigned int[numTris];
	m_v2 = new unsigned int[numTris];
	m_UV0 = new unsigned int[numTris];
	m_UV1 = new unsigned int[numTris];
	m_UV2 = new unsigned int[numTris];
	m_NumTris = numTris;
}

void TriList::FreeTris()
{
	if( m_v0 )
	{
		delete m_v0;
		m_v0 = NULL;
	}
	if( m_v1 )
	{
		delete m_v1;
		m_v1 = NULL;
	}
	if( m_v2 )
	{
		delete m_v2;
		m_v2 = NULL;
	}
	if( m_UV0 )
	{
		delete m_UV0;
		m_UV0 = NULL;
	}
	if( m_UV1 )
	{
		delete m_UV1;
		m_UV1 = NULL;
	}
	if( m_UV2 )
	{
		delete m_UV2;
		m_UV2 = NULL;
	}

	m_NumTris = 0;
}