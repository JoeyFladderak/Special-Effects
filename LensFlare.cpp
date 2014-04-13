#include "LensFlare.h"
#include "GLextensions.h"
#include "GLErrorHelper.h"
#include <gl/GLU.h>
#include <gl/glext.h>
#include "OcclusionQuery.h"

Lensflare::Lensflare()
{
	// init stuff
	m_Shine = new Texture("Textures/shine.png");
	m_Ring = new Texture("Textures/haloflare.png");
	m_Halo = new Texture("Textures/flare4.png");
	m_Circle = new Texture("Textures/flare4.png");
	m_Point = new Texture("Textures/flare5.png");

	for(int i = 0; i < NUMQUERY; i++)
		m_OcclusionQuery[i] = new OcclusionQuery();

	m_QueryCounter = 0;
}

Lensflare::~Lensflare()
{

}

void Lensflare::Update()
{
	if(m_Visibility < 0.01f)
		return;

	vector3 pos;

	glEnable2D();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);
	glDisable(GL_DEPTH_TEST);

	glEnable(GL_TEXTURE_2D);
	glColor4f(1.0f, 1.0f, 1.0f, m_Visibility);

	int ring = m_Ring->GetOpenGLHandle();
	int halo = m_Halo->GetOpenGLHandle();
	int circle = m_Circle->GetOpenGLHandle();
	int point = m_Point->GetOpenGLHandle();
	int shine = m_Shine->GetOpenGLHandle();

	static const float Offsets[NUM_FLARES] = {-1.2f,	-0.95f, -0.5f,	-0.30f,	-0.30f,	-0.25f,	-0.1f, 
		0.20f,	0.35f,	0.53f,	0.57f,	0.62f,	1.0f,	1.25f};

	static const float Sizes[NUM_FLARES] = {	5.5f,		2.5f,		1,		0.5f,	2 ,		1,		0.1f,		
		0.1f,	0.7f,	0.7f,	1.4f,	0.3f,	3,		1};

	static const int Flares[NUM_FLARES] = {	ring,	halo,	circle,	circle,	halo,	circle,	point,
		point,	circle,	circle,	circle,	circle,	shine,	halo};

	vector3 Colors[NUM_FLARES] = {	vector3(1,1,1),	vector3(0.2f,1,0.2f),// ring, halo
		vector3(1,1,1),	vector3(1,1,0),// circle circle
		vector3(1,1,0), vector3(1,1,0),// halo circle
		vector3(1,1,1),	vector3(1,1,1),// point point
		vector3(1,0.1f,0),	vector3(0.1f,0.6f,1),// circle circle
		vector3(0.1f,0.6f,1), vector3(0.1f,0.6f,1),//circle circle
		vector3(1, 1, 1), vector3(1,1,1)};//shine halo

	float aspect = (float)m_ScreenHeight / (float)m_ScreenWidth;
	int size;
	for(int i = 0; i < NUM_FLARES; i++)
	{
		glColor4f(Colors[i].x, Colors[i].y, Colors[i].z, 0.5 * m_Visibility);
		size = 0.1f * Sizes[i];
		pos = m_Direction * Offsets[i] * m_Length;
		glBindTexture(GL_TEXTURE_2D, Flares[i]);
		glBegin(GL_QUADS);
			glTexCoord2f(0,0);
			glVertex2f(pos.x-0.1,pos.y-0.1 / aspect);
			glTexCoord2f(1,0);
			glVertex2f(pos.x+0.1,pos.y-0.1 / aspect);
			glTexCoord2f(1,1);
			glVertex2f(pos.x+0.1,pos.y+0.1 / aspect);
			glTexCoord2f(0,1);
			glVertex2f(pos.x-0.1,pos.y+0.1 / aspect);
		glEnd();
	}

	glDisable2D();

	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glBindTexture(GL_TEXTURE_2D,0);
	glDisable(GL_TEXTURE_2D);
}

void Lensflare::ComputeScreenCoordinates( vector3 a_Position )
{
	glGetDoublev(GL_MODELVIEW_MATRIX, m_ModelView);
	glGetDoublev(GL_PROJECTION_MATRIX, m_Projection);
	glGetIntegerv(GL_VIEWPORT, m_ViewPort);	

	m_ScreenCenter.x = (m_ScreenWidth - 1) / 2;
	m_ScreenCenter.y = (m_ScreenHeight - 1) / 2;

	double temp[3];
	gluProject(a_Position.x, a_Position.y, a_Position.z, m_ModelView, m_Projection, m_ViewPort, &temp[0], &temp[1], &temp[2]);

	m_LightScreenPos.x = (float)temp[0];
	m_LightScreenPos.y = (float)m_ScreenHeight - (float)temp[1];
	m_LightScreenPos.z = (float)temp[2];

	m_LightScreenPos.x = (m_LightScreenPos.x * 2) / m_ScreenWidth - 1;
	m_LightScreenPos.y = (m_LightScreenPos.y * 2) / m_ScreenHeight - 1;

	m_Direction = vector3(m_LightScreenPos.x, m_LightScreenPos.y, 0);

	m_Length = m_Direction.Length();
	m_Direction.x /= m_Length;
	m_Direction.y /= m_Length;
}

void Lensflare::DrawFlare( float a_Offset )
{

}

void Lensflare::SetScreenDimensions( int m_X, int m_Y )
{
	m_ScreenWidth = m_X;
	m_ScreenHeight = m_Y;
}

void Lensflare::glEnable2D(  ) 
{  
	glMatrixMode( GL_PROJECTION ); 

	glPushMatrix(); 
	glLoadIdentity(); 

	glOrtho( -1, 1,	1, -1, -1, 1 );  
	glMatrixMode( GL_MODELVIEW ); 

	glPushMatrix(); 
	glLoadIdentity();

	glPushAttrib( GL_DEPTH_BUFFER_BIT | GL_LIGHTING_BIT );  
	glDisable( GL_DEPTH_TEST );  
	glDisable( GL_LIGHTING ); 
} 

void Lensflare::glDisable2D(  ) 
{  
	glPopAttrib(); 
	glMatrixMode( GL_PROJECTION ); 

	glPopMatrix();
	glMatrixMode( GL_MODELVIEW ); 

	glPopMatrix(); 
}

void Lensflare::DetermineVisibility( vector3 a_Position )
{
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	glDepthMask(GL_FALSE);

	m_OcclusionQuery[m_QueryCounter]->Start();
	glBindTexture(GL_TEXTURE_2D,0);
	glDisable( GL_TEXTURE_2D );
	glDisable(GL_LIGHTING);
	glPointSize(64);
	glPushMatrix();
	glTranslatef(a_Position.x, a_Position.y, a_Position.z);
	glColor3f(1,1,1);
	glBegin(GL_POINTS);
	glVertex3f(0,0,0);
	glEnd();
	glPopMatrix();

	m_OcclusionQuery[m_QueryCounter]->End();

	m_QueryCounter = (m_QueryCounter + 1) % NUMQUERY;
	unsigned int pixelsdrawn;
	pixelsdrawn = m_OcclusionQuery[m_QueryCounter]->GetNumPixels();
	float maxarea = (63.0f * 63.0f);
	m_Visibility = (float)pixelsdrawn / maxarea;

	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glDepthMask(GL_TRUE);
}
