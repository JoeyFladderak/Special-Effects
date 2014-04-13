#pragma once

#include "vector3.h"
#include "BaseModel.h"
#include <OpenMDL.h>
#include <Cg/cg.h>
#include <Cg/cgGL.h>
#include "Shader.h"
#include "Texture.h"
#include <vector>

class OpenMDLmodel: public BaseModel
{
public:
	OpenMDLmodel(void);
	virtual ~OpenMDLmodel(void);

	void SetInfo( const vector3& camPos, const vector3& lightInWorldSpace, const vector3& lightColor, GLfloat worldToCam[16], float ambient, const vector3& clipPlane_point, const vector3& clipPlane_normal );
	virtual void Render( double elapsed );
	void RenderWithoutDepth();
	void LoadFromFile( CGcontext cgContex, const char* filename, float vertexScaleFactor );

	void GetAABBWorldSpace( vector3& minExtent, vector3& maxExtent );

protected:
	void EnableMaterial( OpenMDL::Material* material );
	void DisableMaterial( OpenMDL::Material* material );
	void LoadTextures();
	void BindTextureByName( const std::string& textureName );
	void InitUberShader( CGcontext cgContex );

	OpenMDL::Scene* m_Scene;
	OpenMDL::Importer m_Importer;

	Shader m_UberShader;
	CGparameter m_UberShader_ModelCamProj;
	CGparameter m_UberShader_ModelCam;
	CGparameter m_UberShader_Lightpos_Pixel;
	CGparameter m_UberShader_Ambient;
	CGparameter m_UberShader_CamPos;
	CGparameter m_UberShader_bSpecularMapping;
	CGparameter m_UberShader_WorldCam;
	CGparameter m_UberShader_LightColor;
	CGparameter m_UberShader_SpecularPower;
	CGparameter m_UberShader_ClipPlane_Point;
	CGparameter m_UberShader_ClipPlane_Normal;


	std::vector< std::pair< std::string, Texture* > > m_TextureList;

	vector3 m_CamPos;
	vector3 m_LightInWorldSpace;
	vector3 m_LightColor;
	GLfloat m_WorldCam[16];
	float m_Ambient;
	vector3 m_ClipPlane_Point;
	vector3 m_ClipPlane_Normal;

	float m_VertexScaleFactor;

	bool m_bDisplayListsGenerated;
	std::vector< GLuint > m_DisplayLists;
};
