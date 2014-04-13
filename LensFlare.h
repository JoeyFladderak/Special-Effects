#ifndef _LENSFLARE_H_
#define _LENSFLARE_H_

#include "vector3.h"
#include "Texture.h"
#include "OcclusionQuery.h"

#define NUM_FLARES 14
#define NUMQUERY 10

class Lensflare
{
public:

	Lensflare();
	~Lensflare();

	void Update();
	void ComputeScreenCoordinates(vector3 a_Position);
	void DrawFlare(float a_Offset);
	void SetScreenDimensions(int m_X, int m_Y);
	void DetermineVisibility(vector3 a_Position);

protected:

	void glEnable2D();
	void glDisable2D();

	double m_ModelView[16];
	double m_Projection[16];
	int m_ViewPort[4];

	vector3 m_ScreenCenter;
	vector3 m_LightScreenPos;
	vector3 m_Direction;
	float m_Length;

	int m_ScreenWidth;
	int m_ScreenHeight;

	Texture* m_Shine;
	Texture* m_Ring;
	Texture* m_Halo;
	Texture* m_Circle;
	Texture* m_Point;

	OcclusionQuery* m_OcclusionQuery[NUMQUERY];
	int m_QueryCounter;
	float m_Visibility;
};
#endif