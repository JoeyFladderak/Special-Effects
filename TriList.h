#pragma once

class TriList
{
public:
	TriList(void);
	~TriList(void);

	void AllocateTris( unsigned int numTris );
	void FreeTris();

	inline unsigned int GetNumTris() { return m_NumTris; };

	inline void SetTri( unsigned int index, unsigned int v0, unsigned int v1, unsigned int v2, unsigned int uv0, unsigned int uv1, unsigned int uv2 ) 
	{
		m_v0[index] = v0;
		m_v1[index] = v1;
		m_v2[index] = v2;
		m_UV0[index] = uv0;
		m_UV1[index] = uv1;
		m_UV2[index] = uv2;
	}
	inline unsigned int GetTriV0( unsigned int index ) { return m_v0[index]; }
	inline unsigned int GetTriV1( unsigned int index ) { return m_v1[index]; }
	inline unsigned int GetTriV2( unsigned int index ) { return m_v2[index]; }
	inline unsigned int GetTriUV0( unsigned int index ) { return m_UV0[index]; }
	inline unsigned int GetTriUV1( unsigned int index ) { return m_UV1[index]; }
	inline unsigned int GetTriUV2( unsigned int index ) { return m_UV2[index]; }

	inline unsigned int* GetTriV0list() { return m_v0; }
	inline unsigned int* GetTriV1list() { return m_v1; }
	inline unsigned int* GetTriV2list() { return m_v2; }
	inline unsigned int* GetTriUV0list() { return m_UV0; }
	inline unsigned int* GetTriUV1list() { return m_UV1; }
	inline unsigned int* GetTriUV2list() { return m_UV2; }

protected:
	unsigned int m_NumTris;

	unsigned int* m_v0;
	unsigned int* m_v1;
	unsigned int* m_v2;
	unsigned int* m_UV0;
	unsigned int* m_UV1;
	unsigned int* m_UV2;
};
