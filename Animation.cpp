#include "Animation.h"
#include "Vector3List.h"
#include "mathHelper.h"

Animation::Animation(void)
	:m_StartFrame(0)
	,m_EndFrame(0)
	,m_FPS(1.0f)
	,m_CurrentFrame(0)
	,m_NextFrame(0)
	,m_InterpolationValue(0.0f)
	,m_Looping(true)
{
}

Animation::~Animation(void)
{
}

void Animation::SetFrames( unsigned int startFrame, unsigned int endFrame )
{
	m_StartFrame = startFrame;
	m_EndFrame = endFrame;
}

void Animation::Update( float a_DT, Vector3List* a_VertexList[] )
{
	m_InterpolationValue += m_FPS * a_DT;

	while(m_InterpolationValue > 1.0f)
	{
		m_CurrentFrame = m_NextFrame;
		m_NextFrame++;
		m_InterpolationValue -= 1.0f;

		if(m_NextFrame > m_EndFrame)
			m_NextFrame = m_StartFrame;
	}

	m_CurrentVertexX = a_VertexList[m_CurrentFrame]->GetXlist();
	m_CurrentVertexY = a_VertexList[m_CurrentFrame]->GetYlist();
	m_CurrentVertexZ = a_VertexList[m_CurrentFrame]->GetZlist();

	m_NextVertexX = a_VertexList[m_NextFrame]->GetXlist();
	m_NextVertexY = a_VertexList[m_NextFrame]->GetYlist();
	m_NextVertexZ = a_VertexList[m_NextFrame]->GetZlist();
}

void Animation::UpdateNormals( float a_DT, Vector3List* a_VertexNormalList[] )
{
	m_CurrentVertexX = a_VertexNormalList[m_CurrentFrame]->GetXlist();
	m_CurrentVertexY = a_VertexNormalList[m_CurrentFrame]->GetYlist();
	m_CurrentVertexZ = a_VertexNormalList[m_CurrentFrame]->GetZlist();

	m_NextVertexX = a_VertexNormalList[m_NextFrame]->GetXlist();
	m_NextVertexY = a_VertexNormalList[m_NextFrame]->GetYlist();
	m_NextVertexZ = a_VertexNormalList[m_NextFrame]->GetZlist();
}


void Animation::Interpolate( unsigned int a_NumVectors, float* a_XList, float* a_YList, float* a_ZList )
{
	for(int i = 0; i < a_NumVectors; i++)
	{
		a_XList[i] = MathHelper::Lerp(m_CurrentVertexX[i], m_NextVertexX[i], m_InterpolationValue);
		a_YList[i] = MathHelper::Lerp(m_CurrentVertexY[i], m_NextVertexY[i], m_InterpolationValue);
		a_ZList[i] = MathHelper::Lerp(m_CurrentVertexZ[i], m_NextVertexZ[i], m_InterpolationValue);
	}
}

void Animation::BlendInterpolate( unsigned int a_NumVectors, float* a_XList, float* a_YList, float* a_ZList, float* a_XBlendList, float* a_YBlendList, float* a_ZBlendList, float a_BlendFactor )
{
	for(int i = 0; i < a_NumVectors; i++)
	{
		a_XList[i] = MathHelper::Lerp(a_XList[i], a_XBlendList[i], a_BlendFactor);
		a_YList[i] = MathHelper::Lerp(a_YList[i], a_YBlendList[i], a_BlendFactor);
		a_ZList[i] = MathHelper::Lerp(a_ZList[i], a_ZBlendList[i], a_BlendFactor);
	}
}
