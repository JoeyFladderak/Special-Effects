#ifndef _HSV_H_
#define _HSV_H_

#include <Cg/cg.h>
#include <Cg/cgGL.h>
#include "Shader.h"

class HSV
{
public:

	HSV();
	~HSV();

	void Init(CGcontext a_CGcontext, const char* a_Shader);
	void Enable();
	void Disable();

	void UpdateParameters();

	void SetHue(const float& a_Hue) {m_Hue = a_Hue;}
	void SetSaturation(const float& a_Saturation) {m_Saturation = a_Saturation;}
	void SetValue(const float& a_Value) {m_Value = a_Value;}

	const float& GetHue() const {return m_Hue;}
	const float& GetSaturation() const {return m_Saturation;}
	const float& GetValue() const {return m_Value;}


protected:

	Shader m_HSV;
	CGparameter m_HSV_FrameBuffer;
	CGparameter m_HSV_H;
	CGparameter m_HSV_S;
	CGparameter m_HSV_V;

	float m_Hue;
	float m_Saturation;
	float m_Value;
};
#endif