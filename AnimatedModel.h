#pragma once


#include "vector3.h"
#include <Cg/cg.h>
#include <Cg/cgGL.h>
#include "BaseModel.h"
#include "Animation.h"
#include "Texture.h"

#define MAX_FRAMES 512
#define NUM_ANIMATIONS 20

class Vector3List;
class TriList;
class UVlist;

class AnimatedModel: public BaseModel
{
public:
	AnimatedModel(void);
	~AnimatedModel(void);

	enum AnimationType
	{
		eReady,
		eRun,
		eAttack,
		eHitA,
		eHitB,
		eHitC,
		eJump,
		eFlip,
		eSalto,
		eTaunt,
		eWave,
		ePoint,
		eCrouchReady,
		eCrouchRun,
		eCrouchAttack,
		eCrouchHit,
		eCrouchDie,
		eDieA,
		eDieB,
		eDieC,
		eUnknown
	};

	void BindTexture();
	void Render( double elapsed );
	void SetNumFrames( unsigned int numFrames );
	void SetNumVertices( unsigned int numVertices );
	void SetNumUVs( unsigned int numUVs );
	void SetNumFaces( unsigned int numFaces );
	void SetVertex( unsigned int frameNo, unsigned int index, float x, float y, float z );
	void SetFace( unsigned int index, unsigned int v0, unsigned int v1, unsigned int v2, unsigned int uv0, unsigned int uv1, unsigned int uv2 );
	void SetUV( unsigned int index, float u, float v );
	unsigned int GetNumVertices();
	unsigned int GetNumFaces();
	void SetTexture( Texture* skin );
	void SetAnimationFrames( AnimationType animType, unsigned int startFrame, unsigned int endFrame );
	void SetAnimation(AnimationType a_AnimationType);

	bool Crossfade(AnimationType a_AnimationType, float a_Factor);

	float GetU( int index );
	float GetV( int index );

	void CalcNormals();

	bool IsBlending() {return m_Blending;}

protected:

	unsigned int m_numFrames;
	Vector3List* m_VertexList[MAX_FRAMES];
	Vector3List* m_VertexNormalList[MAX_FRAMES];
	Vector3List* m_FaceNormalList[MAX_FRAMES];
	TriList* m_TriList;
	UVlist* m_UVlist;
	Animation m_Animations[NUM_ANIMATIONS];
	Animation m_ActiveAnimation;
	Animation m_BlendToAnimation;

	AnimationType m_CurrentAnimationType;
	bool m_Blending;
	float m_CurrentBlendTime;

	Texture* m_texture;
};
