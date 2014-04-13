#pragma once

#include "vector3.h"

class BaseModel
{
public:
	BaseModel(void);
	virtual ~BaseModel(void);

	void SetPosition( const vector3& pos ) { m_Position = pos; }
	void SetEulerAngles( const vector3& eulers ) { m_EulerAngles = eulers; }

	const vector3& GetPosition() { return m_Position; }
	const vector3& GetEulerAngles() { return m_EulerAngles; }

	void SetTransform();

	virtual void Render( double elapsed ) = 0;

protected:
	vector3 m_Position;
	vector3 m_EulerAngles;

};
