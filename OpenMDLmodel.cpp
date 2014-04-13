#include <windows.h>
#include <gl/gl.h>
#include <gl/glext.h>
#include "OpenMDLmodel.h"
#include "GLextensions.h"
#include <assert.h>

OpenMDLmodel::OpenMDLmodel(void)
{
	m_Scene = NULL;
	m_bDisplayListsGenerated = false;
}


OpenMDLmodel::~OpenMDLmodel(void)
{
}


void OpenMDLmodel::SetInfo( const vector3& camPos, const vector3& lightInWorldSpace, const vector3& lightColor, GLfloat worldToCam[16], float ambient, const vector3& clipPlane_point, const vector3& clipPlane_normal )
{
	m_CamPos = camPos;
	m_LightInWorldSpace = lightInWorldSpace;
	m_LightColor = lightColor;
	for( int i=0;i<16;i++)
	{
		m_WorldCam[i] = worldToCam[i];
	}
	m_Ambient = ambient;

	m_ClipPlane_Point = clipPlane_point;
	m_ClipPlane_Normal = clipPlane_normal;
}



void OpenMDLmodel::Render( double elapsed )
{
	glPushMatrix();
	SetTransform();

	// Get a mesh count
	unsigned int meshCount = m_Scene->GetMeshes().Size();

	for(unsigned int m = 0; m < meshCount; m++)
	{
		// retrieve mesh data
		OpenMDL::Mesh* mesh = m_Scene->GetMeshes()[m];

		
		assert( mesh->GetMaterials().Size() == 1 );


		EnableMaterial(  mesh->GetMaterial(0) );

		unsigned int vertexCount = mesh->GetVertexIndexCount();

	
		if( !m_bDisplayListsGenerated )
		{
			GLuint dlist = glGenLists(1);
			m_DisplayLists.push_back( dlist );
			glNewList( dlist, GL_COMPILE );
		
			glBegin( GL_TRIANGLES );
			{
				int currentVertex=0;
				for( unsigned int i=0;i<vertexCount/3; i++ )
				{
					for( int j=0;j<3;j++ )
					{
						currentVertex = i*3 + (j % 3);

						unsigned int vertexIndex = mesh->GetVertexIndices()[currentVertex];
						unsigned int normalIndex = mesh->GetNormalIndices()[currentVertex];
						unsigned int uvIndex = mesh->GetTexCoordIndices(0)[currentVertex];

						OpenMDL::float3& vertex = mesh->GetVertices()[vertexIndex];
						OpenMDL::float3& normal = mesh->GetNormals()[normalIndex];
						OpenMDL::float2& uv = mesh->GetTexCoordSet(0)[uvIndex];

						glNormal3f( normal.x, normal.y, normal.z );
						glTexCoord2f( uv.x, uv.y );

						glVertex3f( vertex.x /m_VertexScaleFactor, vertex.y  /m_VertexScaleFactor , vertex.z /m_VertexScaleFactor );
					}
				}
			}
			glEnd();

			glEndList();
		}
		else
		{
			glCallList( m_DisplayLists[m] );
		}

		DisableMaterial(  mesh->GetMaterial(0) );
	}

	m_bDisplayListsGenerated = true;

	glEnable( GL_TEXTURE_2D );
	glPopMatrix();
}


void OpenMDLmodel::LoadFromFile( CGcontext cgContex, const char* filename, float vertexScaleFactor )
{
	m_VertexScaleFactor = vertexScaleFactor;
	InitUberShader( cgContex );

	// load a scene file
	bool newInstance = false;
	m_Scene = m_Importer.LoadScene(filename, &newInstance);
	// NOTE: newInstance will be true if the scene file was loaded for the first time.

	LoadTextures();
}


void OpenMDLmodel::InitUberShader( CGcontext cgContex )
{
	m_UberShader.Init( cgContex, "shaders/UberShader");

	m_UberShader_ModelCamProj = cgGetNamedParameter( m_UberShader.GetVertexProgram(), "ModelCamProj" );
	m_UberShader_ModelCam = cgGetNamedParameter( m_UberShader.GetVertexProgram(), "ModelCam" );

	m_UberShader_WorldCam = cgGetNamedParameter( m_UberShader.GetPixelProgram(), "WorldCam" );
	m_UberShader_Lightpos_Pixel = cgGetNamedParameter( m_UberShader.GetPixelProgram(), "lightPosInWorldSpace" );
	m_UberShader_Ambient = cgGetNamedParameter( m_UberShader.GetPixelProgram(), "ambient" );
	m_UberShader_CamPos = cgGetNamedParameter( m_UberShader.GetPixelProgram(), "camPosInWorldSpace" );
	m_UberShader_LightColor = cgGetNamedParameter( m_UberShader.GetPixelProgram(), "lightColor" );

	m_UberShader_bSpecularMapping = cgGetNamedParameter( m_UberShader.GetPixelProgram(), "bSpecularMapping" );
	m_UberShader_SpecularPower = cgGetNamedParameter( m_UberShader.GetPixelProgram(), "specularPower" );

	m_UberShader_ClipPlane_Point = cgGetNamedParameter( m_UberShader.GetPixelProgram(), "clipPlane_point" );
	m_UberShader_ClipPlane_Normal = cgGetNamedParameter( m_UberShader.GetPixelProgram(), "clipPlane_normal" );
}


void OpenMDLmodel::EnableMaterial( OpenMDL::Material* material )
{
	OpenMDL::String fullname;

	if( material->HasDiffuseMap() )
	{
		glActiveTextureARB( GL_TEXTURE0_ARB );
		glEnable( GL_TEXTURE_2D );
		char tmps[256];
		sprintf( tmps, "Textures/%s",  material->GetDiffuseMap().GetBuffer() );
		BindTextureByName( tmps );
	}

	glActiveTextureARB( GL_TEXTURE1_ARB );
	if( material->HasSpecularMap() )
	{
		glEnable( GL_TEXTURE_2D );
		fullname = OpenMDL::String("Textures/") + material->GetSpecularMap();
		BindTextureByName( fullname.GetBuffer() );
		cgSetParameter1i( m_UberShader_bSpecularMapping, 1 );
	}
	else
	{
		glDisable( GL_TEXTURE_2D );
		cgSetParameter1i( m_UberShader_bSpecularMapping, 0 );
	}

/*
	glActiveTextureARB( GL_TEXTURE2_ARB );
	if( material->HasNormalMap() )
	{
		glEnable( GL_TEXTURE_2D );
		fullname = OpenMDL::String("Textures/") + material->GetNormalMap();
		BindTextureByName( fullname.GetBuffer() );
		cgSetParameter1i( m_UberShader_bNormalMapping, 1 );
	}
	else
	{
		glDisable( GL_TEXTURE_2D );
		cgSetParameter1i( m_UberShader_bNormalMapping, 0 );
	}
*/

	glActiveTextureARB( GL_TEXTURE0_ARB );

	cgGLSetStateMatrixParameter( m_UberShader_ModelCamProj, CG_GL_MODELVIEW_PROJECTION_MATRIX, CG_GL_MATRIX_IDENTITY );
	cgGLSetStateMatrixParameter( m_UberShader_ModelCam, CG_GL_MODELVIEW_MATRIX, CG_GL_MATRIX_IDENTITY );
	cgGLSetParameter3f( m_UberShader_Lightpos_Pixel, m_LightInWorldSpace.x, m_LightInWorldSpace.y, m_LightInWorldSpace.z );
	cgGLSetParameter1f( m_UberShader_Ambient, m_Ambient );
	cgGLSetParameter3f( m_UberShader_CamPos, m_CamPos.x, m_CamPos.y, m_CamPos.z );
	cgGLSetParameter3f( m_UberShader_LightColor, m_LightColor.x, m_LightColor.y, m_LightColor.z );
	cgGLSetMatrixParameterfc( m_UberShader_WorldCam, m_WorldCam );
	cgGLSetParameter1f( m_UberShader_SpecularPower, material->GetSpecularCoeff() );
	cgGLSetParameter3f( m_UberShader_ClipPlane_Point, m_ClipPlane_Point.x, m_ClipPlane_Point.y, m_ClipPlane_Point.z );
	cgGLSetParameter3f( m_UberShader_ClipPlane_Normal, m_ClipPlane_Normal.x, m_ClipPlane_Normal.y, m_ClipPlane_Normal.z );

	m_UberShader.EnablePrograms();
}


void OpenMDLmodel::DisableMaterial( OpenMDL::Material* material )
{
	m_UberShader.DisablePrograms();
}


void OpenMDLmodel::LoadTextures()
{
	for( unsigned int i=0; i<m_Scene->GetMaterials().Size(); i++ )
	{
		OpenMDL::Material* mat = m_Scene->GetMaterials()[i];

		if( mat->HasDiffuseMap() )
		{
			char tmps[256];
			sprintf( tmps, "Textures/%s", mat->GetDiffuseMap().GetBuffer() );
			Texture* texture = new Texture( tmps );

			std::pair< std::string, Texture* > entry( tmps, texture);
			m_TextureList.push_back( entry );
		}
		if( mat->HasSpecularMap() )
		{
			OpenMDL::String s = OpenMDL::String("Textures/") + mat->GetSpecularMap();
			Texture* texture = new Texture( s.GetBuffer() );

			std::pair< std::string, Texture* > entry(s.GetBuffer(),texture);
			m_TextureList.push_back( entry );
		}

/*
		if( mat->HasNormalMap() )
		{
			OpenMDL::String s = OpenMDL::String("Textures/") + mat->GetNormalMap();
			Texture* texture = new Texture( s.GetBuffer() );

			std::pair< std::string, Texture* > entry(s.GetBuffer(),texture);
			m_TextureList.push_back( entry );

		}
*/

	}
}


void OpenMDLmodel::BindTextureByName( const std::string& textureName )
{
	std::vector< std::pair< std::string, Texture* > >::iterator it = m_TextureList.begin();

	while(it != m_TextureList.end() )
	{
		std::pair< std::string, Texture* > entry = (*it);

		if( entry.first.compare( textureName ) == 0 )
		{
			entry.second->BindTexture();
			return;
		}

		it++;
	}

	assert(0);	// if we got here, we're trying to bind a texture that was not loaded!
}


void OpenMDLmodel::GetAABBWorldSpace( vector3& minExtent, vector3& maxExtent )
{
	// Get a mesh count
	unsigned int meshCount = m_Scene->GetMeshes().Size();

	minExtent.x = 1e30f;
	minExtent.y = 1e30f;
	minExtent.z = 1e30f;

	maxExtent = -minExtent;

	for(unsigned int m = 0; m < meshCount; m++)
	{
		// retrieve mesh data
		OpenMDL::Mesh* mesh = m_Scene->GetMeshes()[m];
		unsigned int vertexCount = mesh->GetVertexIndexCount();

		int currentVertex=0;
		for( unsigned int i=0;i<vertexCount/3; i++ )
		{
			for( int j=0;j<3;j++ )
			{
				currentVertex = i*3 + (j % 3);

				unsigned int vertexIndex = mesh->GetVertexIndices()[currentVertex];

				OpenMDL::float3 vertex = mesh->GetVertices()[vertexIndex];
				vertex.x /= m_VertexScaleFactor;
				vertex.y /= m_VertexScaleFactor;
				vertex.z /= m_VertexScaleFactor;

				if( vertex.x < minExtent.x )
				{
					minExtent.x = vertex.x;
				}
				if( vertex.y < minExtent.y )
				{
					minExtent.y = vertex.y;
				}
				if( vertex.z < minExtent.z )
				{
					minExtent.z = vertex.z;
				}

				if( vertex.x > maxExtent.x )
				{
					maxExtent.x = vertex.x;
				}
				if( vertex.y > maxExtent.y )
				{
					maxExtent.y = vertex.y;
				}
				if( vertex.z > maxExtent.z )
				{
					maxExtent.z = vertex.z;
				}
			}
		}
	}
}

void OpenMDLmodel::RenderWithoutDepth()
{
	// Get a mesh count
	unsigned int meshCount = m_Scene->GetMeshes().Size();

	for(unsigned int m = 0; m < meshCount; m++)
	{
		// retrieve mesh data
		OpenMDL::Mesh* mesh = m_Scene->GetMeshes()[m];

		unsigned int vertexCount = mesh->GetVertexIndexCount();


		if( !m_bDisplayListsGenerated )
		{
			GLuint dlist = glGenLists(1);
			m_DisplayLists.push_back( dlist );
			glNewList( dlist, GL_COMPILE );

			glBegin( GL_TRIANGLES );
			{
				int currentVertex=0;
				for( unsigned int i=0;i<vertexCount/3; i++ )
				{
					for( int j=0;j<3;j++ )
					{
						currentVertex = i*3 + (j % 3);

						unsigned int vertexIndex = mesh->GetVertexIndices()[currentVertex];
						unsigned int normalIndex = mesh->GetNormalIndices()[currentVertex];
						unsigned int uvIndex = mesh->GetTexCoordIndices(0)[currentVertex];

						OpenMDL::float3& vertex = mesh->GetVertices()[vertexIndex];
						OpenMDL::float3& normal = mesh->GetNormals()[normalIndex];
						OpenMDL::float2& uv = mesh->GetTexCoordSet(0)[uvIndex];

						glNormal3f( normal.x, normal.y, normal.z );
						glTexCoord2f( uv.x, uv.y );

						glVertex3f( vertex.x /m_VertexScaleFactor, vertex.y  /m_VertexScaleFactor , vertex.z /m_VertexScaleFactor );
					}
				}
			}
			glEnd();

			glEndList();
		}
		else
		{
			glCallList( m_DisplayLists[m] );
		}

	}

	m_bDisplayListsGenerated = true;

	glEnable( GL_TEXTURE_2D );
}
