#include "Camera.h"

Camera::Camera()
{
	m_location.x = 0.0;
	m_location.y = 0.0;
	m_location.z = 0.0;

	m_orientation.reset();
}


Camera::~Camera()
{
}


void Camera::AddRoll(float angle)
{
	m_orientation.postMult( f00Quat( 0.0, 0.0, angle ) );
}

void Camera::AddYaw(float angle)
{
	m_orientation.postMult( f00Quat( 0.0, angle, 0.0 ) );
}

void Camera::AddPitch(float angle)
{
	m_orientation.postMult( f00Quat( angle, 0.0, 0.0 ) );
}

void Camera::MoveBF(float z)
{
	f00Quat truckQuat( m_orientation );

	// Ask the Quat class for a unit vector, aligned with the orientation.
	GLfloat dirX;
	GLfloat dirY;
	GLfloat dirZ;
	truckQuat.getDirectionVector( dirX, dirY, dirZ );

	// Use the values to update the position....
	m_location.x += dirX * z;
	m_location.y += dirY * z;
	m_location.z += dirZ * z;
}

void Camera::MoveLR(float x)
{
	f00Quat truckQuat( m_orientation );

	truckQuat.postMult( f00Quat( 0.0, -90.0, 0.0 ) );		// Inverted, for her pleasure.

	// Ask the Quat class for a unit vector, aligned with the orientation.
	GLfloat dirX;
	GLfloat dirY;
	GLfloat dirZ;
	truckQuat.getDirectionVector( dirX, dirY, dirZ );

	// Use the values to update the position....
	m_location.x += dirX * x;
	m_location.y += dirY * x;
	m_location.z += dirZ * x;
}

void Camera::MoveUD(float y)
{
	f00Quat truckQuat( m_orientation );

	truckQuat.postMult( f00Quat( 90.0, 0.0, 0.0 ) );		// Inverted, for her pleasure.

	// Ask the Quat class for a unit vector, aligned with the orientation.
	GLfloat dirX;
	GLfloat dirY;
	GLfloat dirZ;
	truckQuat.getDirectionVector( dirX, dirY, dirZ );

	// Use the values to update the position....
	m_location.x += dirX * y;
	m_location.y += dirY * y;
	m_location.z += dirZ * y;
}
