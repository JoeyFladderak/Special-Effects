#pragma once

class UVlist
{
public:
	UVlist(void);
	~UVlist(void);

	void AllocateUVs( unsigned int numUVs );
	void FreeUVs();

	void SetUV( int index, float u, float v );
	inline float GetU( int index ) { return m_U[index]; }
	inline float GetV( int index ) { return m_V[index]; }

	inline float* GetUlist() { return m_U; }
	inline float* GetVlist() { return m_V; }

	inline unsigned int GetNumUVs() { return m_NumUVs; }

protected:
	unsigned int m_NumUVs;

	float* m_U;
	float* m_V;
};
