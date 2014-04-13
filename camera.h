#ifndef CAMERA_H
#define CAMERA_H

#include "vector3.h"
#include "f00Quat.h"

class Camera
{
public:
	Camera();
	~Camera();

	// location methods
	virtual void SetLocation(const vector3& location) {m_location = location;};
	virtual void GetLocation(vector3& location) {location = m_location;};
	virtual vector3 GetLocation() {return m_location;};
	void MoveLR(float x);				// strafe on the X axis (left/right)
	void MoveUD(float y);				// strafe on the Y axis (up/down)
	void MoveBF(float z);				// strafe on the Z axis (back/forth)

	// orientation methods
	virtual void SetOrientation(const f00Quat& orientation) {m_orientation = orientation;};
	virtual void GetOrientation(f00Quat& orientation) {orientation = m_orientation;};
	virtual f00Quat GetOrientation() {return m_orientation;};
	void AddRoll(float angle);
	void AddYaw(float angle);
	void AddPitch(float angle);

protected:
	f00Quat m_orientation;
	vector3 m_location;
};



#endif