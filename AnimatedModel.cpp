#include "mathHelper.h"
#include <windows.h>
#include <gl\gl.h>
#include <assert.h>
#include "AnimatedModel.h"
#include "Vector3List.h"
#include "TriList.h"
#include "UVlist.h"

//#define SHOW_WIREFAME

AnimatedModel::AnimatedModel(void)
{
	for( int i=0;i<MAX_FRAMES; i++ )
	{
		m_VertexList[i] = NULL;
		m_VertexNormalList[i] = NULL;
		m_FaceNormalList[i] = NULL;
	}

	m_TriList = new TriList;
	m_UVlist = new UVlist;
	m_texture = NULL;
	m_Blending = false;
}

AnimatedModel::~AnimatedModel(void)
{
	for( int i=0;i<MAX_FRAMES; i++ )
	{
		if( m_VertexList[i] != NULL )
		{
			delete m_VertexList[i];
		}
	}

	for( int i=0;i<MAX_FRAMES; i++ )
	{
		if( m_VertexNormalList[i] != NULL )
		{
			delete m_VertexNormalList[i];
		}
	}

	for( int i=0;i<MAX_FRAMES; i++ )
	{
		if( m_FaceNormalList[i] != NULL )
		{
			delete m_FaceNormalList[i];
		}
	}

	if( m_TriList != NULL )
	{
		delete m_TriList;
	}
	if( m_UVlist != NULL )
	{
		delete m_UVlist;
	}
}


void AnimatedModel::SetNumFrames( unsigned int numFrames )
{
	m_numFrames = numFrames;

	for( unsigned int i=0; i<m_numFrames; i++ )
	{
		m_VertexList[i] = new Vector3List;
		m_VertexNormalList[i] = new Vector3List;
		m_FaceNormalList[i] = new Vector3List;
	}
}


void AnimatedModel::SetNumVertices( unsigned int numVertices )
{
	assert( m_numFrames > 0 );		// if numframes has not been set yet, crash!
	for( unsigned int i=0; i<m_numFrames; i++ )
	{
		m_VertexList[i]->AllocateVectors( numVertices );
		m_VertexNormalList[i]->AllocateVectors( numVertices );
	}
}


void AnimatedModel::SetNumUVs( unsigned int numUVs )
{
	m_UVlist->AllocateUVs(numUVs);
}


void AnimatedModel::SetNumFaces( unsigned int numFaces )
{
	m_TriList->AllocateTris( numFaces );
	for( unsigned int i=0;i<m_numFrames;i++)
	{
		m_FaceNormalList[i]->AllocateVectors( numFaces );
	}
}

void AnimatedModel::SetVertex( unsigned int frameNo, unsigned int index, float x, float y, float z )
{
	assert( m_numFrames > 0 );		// if numframes has not been set yet, crash!
	assert( frameNo < m_numFrames );
	m_VertexList[frameNo]->SetVector( index, x, y, z );
}

void AnimatedModel::SetFace( unsigned int index, unsigned int v0, unsigned int v1, unsigned int v2, unsigned int uv0, unsigned int uv1, unsigned int uv2 )
{
	m_TriList->SetTri( index, v0, v1, v2, uv0, uv1, uv2 );
}

void AnimatedModel::SetUV( unsigned int index, float u, float v )
{
	m_UVlist->SetUV( index, u, v );
}

unsigned int AnimatedModel::GetNumVertices()
{ 
	return m_VertexList[0]->GetNumVectors();	// vertex count is always the same, so just return frame[0]'s vertex count
}

unsigned int AnimatedModel::GetNumFaces() 
{ 
	return m_TriList->GetNumTris(); 
}

void AnimatedModel::Render( double elapsedTime )
{
	m_ActiveAnimation.Update(elapsedTime, m_VertexList);

	if(m_Blending)
	{
		m_BlendToAnimation.Update(elapsedTime, m_VertexList);
		m_CurrentBlendTime += m_BlendToAnimation.GetFPS() * elapsedTime;
	}

	glEnable( GL_TEXTURE_2D );

	glBegin(GL_TRIANGLES);

	unsigned int* v0List = m_TriList->GetTriV0list();
	unsigned int* v1List = m_TriList->GetTriV1list();
	unsigned int* v2List = m_TriList->GetTriV2list();
	unsigned int* UV0List = m_TriList->GetTriUV0list();
	unsigned int* UV1List = m_TriList->GetTriUV1list();
	unsigned int* UV2List = m_TriList->GetTriUV2list();

	float xList[MAX_FRAMES];
	float yList[MAX_FRAMES];
	float zList[MAX_FRAMES];

	unsigned int numframes = m_VertexList[m_ActiveAnimation.GetCurrentFrame()]->GetNumVectors();
	m_ActiveAnimation.Interpolate(numframes, xList, yList, zList);

	if(m_Blending)
	{
		float tmpX[MAX_FRAMES];
		float tmpY[MAX_FRAMES];
		float tmpZ[MAX_FRAMES];

		m_BlendToAnimation.Interpolate(numframes, tmpX, tmpY, tmpZ);

		m_ActiveAnimation.BlendInterpolate(numframes, xList, yList, zList, tmpX, tmpY, tmpZ, m_CurrentBlendTime);

		if(m_CurrentBlendTime > 1.0f)
		{
			m_ActiveAnimation = m_BlendToAnimation;
			m_Blending = false;
		}
	}

	m_ActiveAnimation.UpdateNormals(elapsedTime, m_VertexNormalList);

	if(m_Blending)
	{
		m_BlendToAnimation.Update(elapsedTime, m_VertexNormalList);
	}

	float xNormalList[MAX_FRAMES];
	float yNormalList[MAX_FRAMES];
	float zNormalList[MAX_FRAMES];

	m_ActiveAnimation.Interpolate(numframes, xNormalList, yNormalList, zNormalList);

	if(m_Blending)
	{
		float tmpX[MAX_FRAMES];
		float tmpY[MAX_FRAMES];
		float tmpZ[MAX_FRAMES];

		m_BlendToAnimation.Interpolate(numframes, tmpX, tmpY, tmpZ);

		m_ActiveAnimation.BlendInterpolate(numframes, xNormalList, yNormalList, zNormalList, tmpX, tmpY, tmpZ, m_CurrentBlendTime);
	}

	for( unsigned int i=0; i<m_TriList->GetNumTris(); i++ )
	{
		glColor3f( 1.0f, 1.0f, 1.0f );

		unsigned int vertex0 = v0List[i];
		unsigned int vertex1 = v1List[i];
		unsigned int vertex2 = v2List[i];
		unsigned int uv0 = UV0List[i];
		unsigned int uv1 = UV1List[i];
		unsigned int uv2 = UV2List[i];

		glNormal3f( xNormalList[vertex0], yNormalList[vertex0], zNormalList[vertex0] );
		glTexCoord2f( m_UVlist->GetU( uv0 ), m_UVlist->GetV( uv0 ) );
		glVertex3f( xList[vertex0], yList[vertex0], zList[vertex0] );

		glNormal3f( xNormalList[vertex1], yNormalList[vertex1], zNormalList[vertex1] );
		glTexCoord2f( m_UVlist->GetU( uv1 ), m_UVlist->GetV( uv1 ) );
		glVertex3f( xList[vertex1], yList[vertex1], zList[vertex1] );

		glNormal3f( xNormalList[vertex2], yNormalList[vertex2], zNormalList[vertex2] );
		glTexCoord2f( m_UVlist->GetU( uv2 ), m_UVlist->GetV( uv2 ) );
		glVertex3f( xList[vertex2], yList[vertex2], zList[vertex2] );


	}

	glEnd();


#ifdef SHOW_WIREFAME
	glDisable( GL_TEXTURE_2D );

	glColor3f( 1.0f, 1.0f, 1.0f );
	for( unsigned int i=0; i<m_TriList->GetNumTris(); i++ )
	{
		glBegin( GL_LINES );

		unsigned int vertex0 = v0List[i];
		unsigned int vertex1 = v1List[i];
		unsigned int vertex2 = v2List[i];
		glVertex3f( xList[vertex0], yList[vertex0], zList[vertex0] );
		glVertex3f( xList[vertex1], yList[vertex1], zList[vertex1] );
		glVertex3f( xList[vertex2], yList[vertex2], zList[vertex2] );

		glEnd();
	}

	glEnable( GL_TEXTURE_2D );
#endif
}

bool AnimatedModel::Crossfade( AnimationType a_AnimationType, float a_Factor )
{
	if(!m_Blending)
	{
		m_BlendToAnimation = m_Animations[a_AnimationType];
		m_BlendToAnimation.GetCurrentFrame() = m_BlendToAnimation.GetStartFrame();	
		m_BlendToAnimation.GetNextFrame() = m_BlendToAnimation.GetStartFrame() + 1;
		m_CurrentBlendTime = 0;
		m_Blending = true;
		return true;
	}	

	return false;
}

void AnimatedModel::CalcNormals()
{
	for( unsigned int frameNo = 0; frameNo < m_numFrames; frameNo++ )
	{
		for( unsigned int i=0; i< m_VertexNormalList[frameNo]->GetNumVectors(); i++ )
		{
			m_VertexNormalList[frameNo]->SetVector( i, 0.0f, 0.0f, 0.0f );
		}

		unsigned int* v0List = m_TriList->GetTriV0list();
		unsigned int* v1List = m_TriList->GetTriV1list();
		unsigned int* v2List = m_TriList->GetTriV2list();

		float* xList = m_VertexList[frameNo]->GetXlist();
		float* yList = m_VertexList[frameNo]->GetYlist();
		float* zList = m_VertexList[frameNo]->GetZlist();

		for( unsigned int i=0; i<m_TriList->GetNumTris(); i++ )
		{
			unsigned int vertex0 = v0List[i];
			unsigned int vertex1 = v1List[i];
			unsigned int vertex2 = v2List[i];

			// calc the normal
			{
				vector3 vvertex0( xList[vertex0], yList[vertex0], zList[vertex0] );
				vector3 vvertex1( xList[vertex1], yList[vertex1], zList[vertex1] );
				vector3 vvertex2( xList[vertex2], yList[vertex2], zList[vertex2] );

				vector3 edgeVec0 = vvertex1 - vvertex0;
				vector3 edgeVec1 = vvertex2 - vvertex0;

				vector3 normal = edgeVec0.Cross( edgeVec1 );
				normal.Normalize();

				m_FaceNormalList[frameNo]->SetVector( i, normal.x, normal.y, normal.z );

				m_VertexNormalList[frameNo]->SetVector( vertex0	, normal.x + m_VertexNormalList[frameNo]->GetX(vertex0) 
														, normal.y + m_VertexNormalList[frameNo]->GetY(vertex0) 
														, normal.z + m_VertexNormalList[frameNo]->GetZ(vertex0) );

				m_VertexNormalList[frameNo]->SetVector( vertex1	, normal.x + m_VertexNormalList[frameNo]->GetX(vertex1) 
														, normal.y + m_VertexNormalList[frameNo]->GetY(vertex1) 
														, normal.z + m_VertexNormalList[frameNo]->GetZ(vertex1) );

				m_VertexNormalList[frameNo]->SetVector( vertex2	, normal.x + m_VertexNormalList[frameNo]->GetX(vertex2) 
														, normal.y + m_VertexNormalList[frameNo]->GetY(vertex2) 
														, normal.z + m_VertexNormalList[frameNo]->GetZ(vertex2) );

			}
		}

		// now normalize all vertex normals
		for( unsigned int i=0; i< m_VertexNormalList[frameNo]->GetNumVectors(); i++ )
		{
			vector3 normal( m_VertexNormalList[frameNo]->GetX( i ), m_VertexNormalList[frameNo]->GetY( i ), m_VertexNormalList[frameNo]->GetZ( i ) );
			normal.Normalize();
			m_VertexNormalList[frameNo]->SetVector( i, normal.x, normal.y, normal.z );
		}
	}
}


void AnimatedModel::SetTexture( Texture* texture )
{
	m_texture = texture;
}


float AnimatedModel::GetU( int index )
{
	return m_UVlist->GetU( index );
}


float AnimatedModel::GetV( int index )
{
	return m_UVlist->GetV( index );
}


void AnimatedModel::SetAnimationFrames( AnimationType animType, unsigned int startFrame, unsigned int endFrame )
{
	m_Animations[ animType ].SetFrames( startFrame, endFrame );
}

void AnimatedModel::BindTexture()
{
	m_texture->BindTexture();
}

void AnimatedModel::SetAnimation( AnimationType a_AnimationType )
{
	m_CurrentAnimationType = a_AnimationType;
	m_ActiveAnimation = m_Animations[m_CurrentAnimationType];

	m_ActiveAnimation.GetCurrentFrame() = m_ActiveAnimation.GetStartFrame();	
	m_ActiveAnimation.GetNextFrame() = m_ActiveAnimation.GetStartFrame() + 1;
}