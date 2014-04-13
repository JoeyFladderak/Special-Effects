#include <stdlib.h>
#include "UVlist.h"

UVlist::UVlist(void)
{
	m_U = NULL;
	m_V = NULL;

	m_NumUVs = 0;
}


UVlist::~UVlist(void)
{
	FreeUVs();
}


void UVlist::AllocateUVs( unsigned int numUVs )
{
	FreeUVs();

	m_U = new float[numUVs];
	m_V = new float[numUVs];

	m_NumUVs = numUVs;
}

void UVlist::FreeUVs()
{
	if( m_U )
	{
		delete[] m_U;
		m_U = NULL;
	}
	if( m_V )
	{
		delete[] m_V;
		m_V = NULL;
	}

	m_NumUVs = 0;
}

void UVlist::SetUV( int index, float u, float v )
{
	m_U[index] = u;
	m_V[index] = v;
}
