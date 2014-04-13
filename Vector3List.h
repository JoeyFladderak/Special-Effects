#pragma once

class Vector3List
{
public:
	Vector3List(void);
	~Vector3List(void);

	void AllocateVectors( unsigned int numVertices );
	void FreeVectors();

	void SetVector( int index, float x, float y, float z );
	inline float GetX( int index ) { return m_X[index]; }
	inline float GetY( int index ) { return m_Y[index]; }
	inline float GetZ( int index ) { return m_Z[index]; }

	inline float* GetXlist() { return m_X; }
	inline float* GetYlist() { return m_Y; }
	inline float* GetZlist() { return m_Z; }

	inline unsigned int GetNumVectors() { return m_NumVectors; }

protected:
	unsigned int m_NumVectors;

	float* m_X;
	float* m_Y;
	float* m_Z;
};
