#include <stdlib.h>
#include "Vector3List.h"

Vector3List::Vector3List(void)
{
	m_X = NULL;
	m_Y = NULL;
	m_Z = NULL;

	m_NumVectors = 0;
}


Vector3List::~Vector3List(void)
{
	FreeVectors();
}


void Vector3List::AllocateVectors( unsigned int numVertices )
{
	FreeVectors();

	m_X = new float[numVertices];
	m_Y = new float[numVertices];
	m_Z = new float[numVertices];

	m_NumVectors = numVertices;
}

void Vector3List::FreeVectors()
{
	if( m_X )
	{
		delete[] m_X;
		m_X = NULL;
	}
	if( m_Y )
	{
		delete[] m_Y;
		m_Y = NULL;
	}
	if( m_Z )
	{
		delete[] m_Z;
		m_Z = NULL;
	}

	m_NumVectors = 0;
}

void Vector3List::SetVector( int index, float x, float y, float z )
{
	m_X[index] = x;
	m_Y[index] = y;
	m_Z[index] = z;
}
