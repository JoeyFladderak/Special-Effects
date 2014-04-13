#pragma once

class Vector3List;
class Animation
{
public:
	Animation(void);
	~Animation(void);

	void SetFrames( unsigned int startFrame, unsigned int endFrame );
	void Update(float a_DT, Vector3List* a_VertexList[]);
	void UpdateNormals(float a_DT, Vector3List* a_VertexNormalList[]);
	void Interpolate(unsigned int a_NumVectors, float* a_XList, float* a_YList, float* a_ZList);
	void BlendInterpolate(unsigned int a_NumVectors, float* a_XList, float* a_YList, float* a_ZList, float* a_XBlendList, float* a_YBlendList, float* a_ZBlendList, float a_BlendFactor);
	
	unsigned int &GetStartFrame() { return m_StartFrame; }
	unsigned int &GetEndFrame() { return m_EndFrame; }
	float &GetFPS() {return m_FPS;}
	unsigned int &GetCurrentFrame() {return m_CurrentFrame;}
	unsigned int &GetNextFrame() {return m_NextFrame;}
	float &GetInterpolationValue() {return m_InterpolationValue;}
	bool &IsLooping() {return m_Looping;}

protected:

	unsigned int m_StartFrame;
	unsigned int m_EndFrame;
	float m_FPS;
	unsigned int m_CurrentFrame;
	unsigned int m_NextFrame;
	float m_InterpolationValue;
	bool m_Looping;

	float* m_CurrentVertexX;
	float* m_CurrentVertexY;
	float* m_CurrentVertexZ;

	float* m_NextVertexX;
	float* m_NextVertexY;
	float* m_NextVertexZ;
};
