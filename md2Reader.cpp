#include "md2Reader.h"
#include "AnimatedModel.h"
#include "scene.h"
#include <string>
#include <assert.h>

using namespace std;

// MD2 loader specific structures follow 
#define MD2_MAX_TRIANGLES		4096
#define MD2_MAX_VERTICES		2048
#define MD2_MAX_TEXCOORDS		2048
#define MD2_MAX_FRAMES			512
#define MD2_MAX_SKINS			32
#define MD2_MAX_FRAMESIZE		(MD2_MAX_VERTICES * 4 + 128)

typedef struct 
{ 
	int magic; 
	int version; 
	int skinWidth; 
	int skinHeight; 
	int frameSize; 
	int numSkins; 
	int numVertices; 
	int numTexCoords; 
	int numTriangles; 
	int numGlCommands; 
	int numFrames; 
	int offsetSkins; 
	int offsetTexCoords; 
	int offsetTriangles; 
	int offsetFrames; 
	int offsetGlCommands; 
	int offsetEnd; 
} md2_header_t;


typedef struct
{
	byte vertex[3];
	byte lightNormalIndex;
} md2_alias_triangleVertex_t;

typedef struct
{
	float vertex[3];
	float normal[3];
	//byte lightNormalIndex;
} md2_triangleVertex_t;

typedef struct
{
	short vertexIndices[3];
	short textureIndices[3];
} md2_triangle_t;

typedef struct
{
	short s, t;
} md2_textureCoordinate_t;

typedef struct
{
	float scale[3];
	float translate[3];
	char name[16];
	md2_alias_triangleVertex_t alias_vertices[1];
} md2_alias_frame_t;

typedef struct
{
	char name[16];
	md2_triangleVertex_t *vertices;
} md2_frame_t;

typedef char md2_skin_t[64];

typedef struct
{
	float s, t;
	int vertexIndex;
} md2_glCommandVertex_t;

struct md2_model_t
{
	md2_header_t			header;
	md2_skin_t				*skins;
	md2_textureCoordinate_t	*texCoords;
	md2_triangle_t			*triangles;
	md2_frame_t				*frames;
	int						*glCommandBuffer;
};



md2Reader::md2Reader()
{
	m_totsize = 0;

	m_File=NULL;
}

md2Reader::~md2Reader()
{
}

int md2Reader::Open( const char filename[] )
{
	fopen_s(&m_File, filename,"rb");
	
	if(!m_File)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}


void md2Reader::md2_freeModel (md2_model_t *model)
{
	if (model)
	{
		if (model->skins)
			free (model->skins);

		if (model->texCoords)
			free (model->texCoords);

		if (model->triangles)
			free (model->triangles);

		if (model->frames)
		{
			int i;

			for (i = 0; i < model->header.numFrames; i++)
			{
				if (model->frames[i].vertices)
					free (model->frames[i].vertices);
			}
			free (model->frames);
		}

		if (model->glCommandBuffer)
			free (model->glCommandBuffer);

		free (model);
	}
}


void md2Reader::Read( AnimatedModel* md2mesh )
{
	m_totsize = 0;
	
	md2_model_t *model;
	byte buffer[MD2_MAX_FRAMESIZE];
	int i;

	model = (md2_model_t *) malloc (sizeof (md2_model_t));
	if (!model)
	{
		MessageBox( NULL, "Unable to allocate memory for model", "Error", MB_ICONSTOP );
		exit(0);
	}

	memset(model,0,sizeof(md2_model_t));


	fread (&model->header, sizeof (md2_header_t), 1, m_File);

	if (model->header.magic != (int) (('2' << 24) + ('P' << 16) + ('D' << 8) + 'I'))
	{
		free(model);
		fclose(m_File);
		MessageBox( NULL, "Invalid md2 model", "Error", MB_ICONSTOP );
		exit(0);
	}

// next, read the skin name (I only support one skin so throw the others away)

	Texture* skinID = NULL;
	bool skinLoaded = false;

	if(model->header.numSkins >= 1)
	{
		fseek(m_File,model->header.offsetSkins,SEEK_SET);
		char filename[64];
		if(fread(filename,64,1,m_File) != 1)
		{
			fclose(m_File);
			MessageBox( NULL, "Unable to read skin name from model", "Error", MB_ICONSTOP );
			exit(0);
		}

		string sFilename(filename);
		int pos = sFilename.find_last_of("/");
		int pos2 = sFilename.find_last_of("\\");
		if(pos2 > pos)
		{
			pos = pos2;
		}

		string shortname = sFilename.substr(pos+1);
		string longname = "Textures\\" + shortname;

		skinID = new Texture( longname.c_str(), GL_LINEAR_MIPMAP_NEAREST, GL_LINEAR, GL_CLAMP, GL_CLAMP );
		skinLoaded = true;
	}

	/* read texture coordinates */
	fseek (m_File, model->header.offsetTexCoords, SEEK_SET);
	if (model->header.numTexCoords > 0)
	{
		model->texCoords = (md2_textureCoordinate_t *) malloc (sizeof (md2_textureCoordinate_t) * model->header.numTexCoords);
		if (!model->texCoords)
		{
			md2_freeModel(model);
			MessageBox( NULL, "Unable to allocate texcoords for md2 model", "Error", MB_ICONSTOP );
			exit(0);
		}

		for (i = 0; i < model->header.numTexCoords; i++)
		{
			if( fread (&model->texCoords[i], sizeof (md2_textureCoordinate_t), 1, m_File) != 1)
			{
				MessageBox( NULL, "Error reading texcoordinates", "Error", MB_ICONSTOP );
			}
		}
	}

	/* read triangles */
	fseek (m_File, model->header.offsetTriangles, SEEK_SET);
	if (model->header.numTriangles > 0)
	{
		model->triangles = (md2_triangle_t *) malloc (sizeof (md2_triangle_t) * model->header.numTriangles);
		if (!model->triangles)
		{
			md2_freeModel (model);
			MessageBox( NULL, "Unable to allocate triangles for md2 model", "Error", MB_ICONSTOP );
			exit(0);
		}

		for (i = 0; i < model->header.numTriangles; i++)
		{
			if( fread (&model->triangles[i], sizeof (md2_triangle_t), 1, m_File) != 1 )
			{
				MessageBox( NULL, "Error reading triangles", "Error", MB_ICONSTOP );
			}
		}
	}

	/* read alias frames */
	fseek (m_File, model->header.offsetFrames, SEEK_SET);
	if (model->header.numFrames > 0)
	{
		model->frames = (md2_frame_t *) malloc (sizeof (md2_frame_t) * model->header.numFrames);
		if (!model->frames)
		{
			md2_freeModel (model);
			MessageBox( NULL, "Unable to allocate frames for md2 model", "Error", MB_ICONSTOP );
			exit(0);
		}

		for (i = 0; i < model->header.numFrames; i++)
		{
			md2_alias_frame_t *frame = (md2_alias_frame_t *) buffer;
			int j;

			model->frames[i].vertices = (md2_triangleVertex_t *) malloc (sizeof (md2_triangleVertex_t) * model->header.numVertices);
			if (!model->frames[i].vertices)
			{
				md2_freeModel (model);
				MessageBox( NULL, "Unable to allocate frame vertices for md2 model", "Error", MB_ICONSTOP );
				exit(0);
			}

			if( fread (frame, 1, model->header.frameSize, m_File) != model->header.frameSize)
			{
				MessageBox( NULL, "Error reading frame", "Error", MB_ICONSTOP );
			}
			strcpy_s (model->frames[i].name, 16, frame->name );
			for (j = 0; j < model->header.numVertices; j++)
			{
				model->frames[i].vertices[j].vertex[0] = (float) ((int) frame->alias_vertices[j].vertex[0]) * frame->scale[0] + frame->translate[0];
				model->frames[i].vertices[j].vertex[2] = -1* ((float) ((int) frame->alias_vertices[j].vertex[1]) * frame->scale[1] + frame->translate[1]);
				model->frames[i].vertices[j].vertex[1] = (float) ((int) frame->alias_vertices[j].vertex[2]) * frame->scale[2] + frame->translate[2];
			}
		}
	}

	fclose(m_File);

// now generate my structures

	md2mesh->SetNumFrames( model->header.numFrames );
	md2mesh->SetNumVertices( model->header.numVertices );

	for( int frameNo=0; frameNo < model->header.numFrames; frameNo++ )
	{
		for( int i=0; i<model->header.numVertices; i++ )
		{
			float scaleFactor = 0.005f;
			md2mesh->SetVertex( frameNo, i, model->frames[frameNo].vertices[i].vertex[0] * scaleFactor, model->frames[frameNo].vertices[i].vertex[1] * scaleFactor, model->frames[frameNo].vertices[i].vertex[2] * scaleFactor );
		}
	}

	md2mesh->SetNumUVs( model->header.numTexCoords );

	for( int i=0;i<model->header.numTexCoords; i++ )
	{
		md2mesh->SetUV( i, (float) model->texCoords[i].s / (float) (model->header.skinWidth), (float)(model->texCoords[ i ].t) / (float) (model->header.skinHeight) );
	}

	md2mesh->SetNumFaces( model->header.numTriangles );
	for( int i=0; i<model->header.numTriangles; i++ )
	{
		md2mesh->SetFace( i, model->triangles[i].vertexIndices[0], model->triangles[i].vertexIndices[2], model->triangles[i].vertexIndices[1],
							 model->triangles[i].textureIndices[0], model->triangles[i].textureIndices[2], model->triangles[i].textureIndices[1] );
	}


	md2mesh->SetTexture( skinID );

	BuildAnimations( model, md2mesh );

	if (model->texCoords)
		free (model->texCoords);

	if (model->triangles)
		free (model->triangles);

	if (model->frames)
	{
		int i;

		for (i = 0; i < model->header.numFrames; i++)
		{
			if (model->frames[i].vertices)
				free (model->frames[i].vertices);
		}
		free (model->frames);
	}

	free(model);
}


void md2Reader::BuildAnimations( md2_model_t *model, AnimatedModel* md2mesh )
{
	AnimatedModel::AnimationType currentType = GetAnimationTypeFromFullName( model->frames[0].name );

	unsigned int startFrame = 0;

	for( int i=1; i < model->header.numFrames; i++ )
	{
		AnimatedModel::AnimationType thisFrameType = GetAnimationTypeFromFullName( model->frames[i].name );
		if( thisFrameType != currentType )
		{
			md2mesh->SetAnimationFrames( currentType, startFrame, i-1 );
			startFrame = i;
			currentType = thisFrameType;
		}
	}

	md2mesh->SetAnimationFrames( currentType, startFrame, model->header.numFrames-1 );
}


AnimatedModel::AnimationType md2Reader::GetAnimationTypeFromFullName( const char* name )
{
	// first, cut off the trailing framenumber

	char nameOnly[256];

	int index = 0;
	while( (name[index] != '\0') && (( name[index] < '0' ) || ( name[index] > '9' )) )
	{
		nameOnly[index] = name[index];
		index++;
	}

	nameOnly[index] = '\0';

	// then, match the short name to an animation type

	if( strcmp( nameOnly, "rdy" ) == 0 )
	{
		return AnimatedModel::eReady;
	}
	else if( strcmp( nameOnly, "run" ) == 0 )
	{
		return AnimatedModel::eRun;
	}
	else if( strcmp( nameOnly, "atk" ) == 0 )
	{
		return AnimatedModel::eAttack;
	}
	else if( strcmp( nameOnly, "hitA" ) == 0 )
	{
		return AnimatedModel::eHitA;
	}
	else if( strcmp( nameOnly, "hitB" ) == 0 )
	{
		return AnimatedModel::eHitB;
	}
	else if( strcmp( nameOnly, "hitC" ) == 0 )
	{
		return AnimatedModel::eHitC;
	}
	else if( strcmp( nameOnly, "jump" ) == 0 )
	{
		return AnimatedModel::eJump;
	}
	else if( strcmp( nameOnly, "flip" ) == 0 )
	{
		return AnimatedModel::eFlip;
	}
	else if( strcmp( nameOnly, "sal" ) == 0 )
	{
		return AnimatedModel::eSalto;
	}
	else if( strcmp( nameOnly, "tnt" ) == 0 )
	{
		return AnimatedModel::eTaunt;
	}
	else if( strcmp( nameOnly, "wav" ) == 0 )
	{
		return AnimatedModel::eWave;
	}
	else if( strcmp( nameOnly, "point" ) == 0 )
	{
		return AnimatedModel::ePoint;
	}
	else if( strcmp( nameOnly, "crd" ) == 0 )
	{
		return AnimatedModel::eCrouchReady;
	}
	else if( strcmp( nameOnly, "crun" ) == 0 )
	{
		return AnimatedModel::eCrouchRun;
	}
	else if( strcmp( nameOnly, "cratk" ) == 0 )
	{
		return AnimatedModel::eCrouchAttack;
	}
	else if( strcmp( nameOnly, "crhit" ) == 0 )
	{
		return AnimatedModel::eCrouchHit;
	}
	else if( strcmp( nameOnly, "crdie" ) == 0 )
	{
		return AnimatedModel::eCrouchDie;
	}
	else if( strcmp( nameOnly, "dieA" ) == 0 )
	{
		return AnimatedModel::eDieA;
	}
	else if( strcmp( nameOnly, "dieB" ) == 0 )
	{
		return AnimatedModel::eDieB;
	}
	else if( strcmp( nameOnly, "dieC" ) == 0 )
	{
		return AnimatedModel::eDieC;
	}
	else
	{
		return AnimatedModel::eUnknown;
	}
}
