#ifndef _BLUR_H_
#define _BLUR_H_

#include <Cg/cg.h>
#include <Cg/cgGL.h>
#include "Shader.h"

class Blur
{
public:

	Blur();
	~Blur();

	void Init(CGcontext a_Context, const char* a_Shader);
	void Enable();
	void Disable();

protected:

	Shader m_HorizontalBlur;
	Shader m_VerticalBlur;
	CGparameter m_HorizontalWeights[32];
	CGparameter m_VerticalWeights[32];
	float m_Weights[32];
};
#endif