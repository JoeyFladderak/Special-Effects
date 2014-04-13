#ifndef _DEPTH_OF_FIELD_H_
#define _DEPTH_OF_FIELD_H_

#include <Cg/cg.h>
#include <Cg/cgGL.h>
#include "Shader.h"
#include "vector3.h"

#define DOFSAMPLES 64

class DepthOfField
{
public:

	DepthOfField();
	~DepthOfField();

	void Init(CGcontext a_Context, const char* a_Shader);
	void Enable();
	void Disable();

	void UpdateParameters();

	void SetMinRange(const float& a_MinRange) {m_TweakMinRange = a_MinRange;}
	void SetMaxRange(const float& a_MaxRange) {m_TweakMaxRange = a_MaxRange;}
	void SetBlurSize(const float& a_BlurSize) {m_TweakBlurSize = a_BlurSize;}

	const float& GetMinRange() const {return m_TweakMinRange;}
	const float& GetMaxRange() const {return m_TweakMaxRange;}
	const float& GetBlurSize() const {return m_TweakBlurSize;}

protected:

	Shader m_DoF;

	CGparameter m_CGSamples[DOFSAMPLES];
	CGparameter m_MinRange;
	CGparameter m_MaxRange;
	CGparameter m_BlurSize;
	
	vector3 m_Samples[DOFSAMPLES];
	float m_TweakMinRange;
	float m_TweakMaxRange;
	float m_TweakBlurSize;

};
#endif