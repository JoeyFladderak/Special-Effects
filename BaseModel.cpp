#include <windows.h>
#include <gl/gl.h>
#include "BaseModel.h"

BaseModel::BaseModel(void)
:m_Position( vector3( 0.0f, 0.0f, 0.0f ) )
,m_EulerAngles( vector3( 0.0f, 0.0f, 0.0f ) )
{
}

BaseModel::~BaseModel(void)
{
}


void BaseModel::SetTransform()
{
	glTranslatef( m_Position.x, m_Position.y, m_Position.z );
	glRotatef( m_EulerAngles.x, 1.0f, 0.0f, 0.0f );
	glRotatef( m_EulerAngles.y, 0.0f, 1.0f, 0.0f );
	glRotatef( m_EulerAngles.z, 0.0f, 0.0f, 1.0f );
}
