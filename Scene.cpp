#include "Scene.h"
#include "Camera.h"
#include "mathHelper.h"
#include "GLextensions.h"
#include "GLErrorHelper.h"
#include "BitmapFont.h"
#include "md2Reader.h"
#include "GL/glext.h"
#include "ParticleSystem.h"
#include <math.h>
#include "matrix.h"

Scene::Scene( unsigned int Xres, unsigned int Yres, bool* keys, HDC hDC, float nearPlane, float farPlane )
	:m_Xres(Xres)
	,m_Yres(Yres)
	,m_CurrentFPS( 0.0f )
	,m_FPSnumFrames( 0 )
	,m_NearPlane( nearPlane )
	,m_FarPlane( farPlane )
	,m_Ambient( 0.0f )
	,m_RenderDebugInfo( true )
{
	m_PreviousKeyDown = new bool[256];
	m_KeysPressed = new bool[256];

	for( int i=0;i<256;i++ )
	{
		m_PreviousKeyDown[i] = false;
		m_KeysPressed[i] = false;
	}

	blendmodes[0] = GL_ZERO;
	blendmodes[1] = GL_ONE;
	blendmodes[2] = GL_SRC_COLOR;
	blendmodes[3] = GL_ONE_MINUS_SRC_COLOR;
	blendmodes[4] = GL_DST_COLOR;
	blendmodes[5] = GL_ONE_MINUS_DST_COLOR;
	blendmodes[6] = GL_SRC_ALPHA;
	blendmodes[7] = GL_ONE_MINUS_SRC_ALPHA;
	blendmodes[8] = GL_DST_ALPHA;
	blendmodes[9] = GL_ONE_MINUS_DST_ALPHA;
	blendmodes[10] = GL_CONSTANT_COLOR;
	blendmodes[11] = GL_ONE_MINUS_CONSTANT_COLOR;
	blendmodes[12] = GL_CONSTANT_ALPHA;
	blendmodes[13] = GL_ONE_MINUS_CONSTANT_ALPHA;
	blendmodes[14] = GL_SRC_ALPHA_SATURATE;
	sfactor = 6;
	dfactor = 7;

	m_Camera = new Camera;
	m_Camera->SetLocation( vector3( -18.625f, 0.14387f, 2.2593f ) );

	m_Particles = new ParticleSystem(200);

	f00Quat orientation;
	orientation.d_val[0] = 0.99389f;
	orientation.d_val[1] = 0.026308f;
	orientation.d_val[2] = -0.10714f;
	orientation.d_val[3] = -0.00253f;

	m_Camera->SetOrientation( orientation );
	m_KeysDown = keys;

	InitExtensions();
	InitCG();
	SetupWorld();
	LoadAndSetupIsland();
	LoadAndSetupBoat();
	LoadAndSetupQ2Model();

	m_FrameBuffer.Init( m_Xres, m_Yres, 1, GL_RGBA8, false );
	m_SecondFrameBuffer.Init(m_Xres, m_Yres, 1, GL_RGBA8, false);
	m_RefractBuffer.Init(m_Xres, m_Yres, 1, GL_RGBA8, false);
	m_ScreenSpaceQuadShader.Init( m_CGcontext, "Shaders\\ScreenSpaceQuadShader" );

	m_Lensflare = new Lensflare();
	m_Lensflare->SetScreenDimensions(m_Xres, m_Yres);

	m_HSV = new HSV();
	m_HSV->Init(m_CGcontext, "Shaders/HSVShader");

	m_DoF = new DepthOfField();
	m_DoF->Init(m_CGcontext, "Shaders/DoF");
	
	m_DoDof = true;
	m_MinRange = m_DoF->GetMinRange();
	m_MaxRange = m_DoF->GetMaxRange();
	m_Blurriness = m_DoF->GetBlurSize();

	m_DoHSV = true;
	m_Hue = m_HSV->GetHue();
	m_Saturation = m_HSV->GetSaturation();
	m_Value = m_HSV->GetValue();

	m_DoParticles = true;
	m_DoLensFlares = true;

	m_Sky = new Sky();
	m_Sky->Init(m_CGcontext, "Shaders/Sky");

	//water
	m_WaterShader.Init(m_CGcontext, "Shaders/Water");
	m_WaterModelCamProj = cgGetNamedParameter(m_WaterShader.GetVertexProgram(), "ModelCamProj");
	m_WaterView = cgGetNamedParameter(m_WaterShader.GetVertexProgram(), "ModelCam");
	m_WaterLightPos = cgGetNamedParameter(m_WaterShader.GetPixelProgram(), "lightPos");
	m_WaterLightCol = cgGetNamedParameter(m_WaterShader.GetPixelProgram(), "lightColor");
	m_WaterCamPos = cgGetNamedParameter(m_WaterShader.GetPixelProgram(), "eyePosW");
	m_WaterUVOffset = cgGetNamedParameter(m_WaterShader.GetPixelProgram(), "uvoffset");
	m_NormalMap.Load("Textures/waternormal.jpg");

	// Font
	m_Font.BuildFont("Arial", 14, hDC );
	m_FPStimer.StartTimer();

	TwInit(TW_OPENGL, NULL);

	m_TweakBar = TwNewBar("TweakBar");
	TwDefine(" GLOBAL help='All tweakable variables' ");
	TwDefine(" TweakBar size='200 400' color='96 216 224' ");
	
	TwAddVarRW(m_TweakBar, "Enable DoF", TW_TYPE_BOOL8, &m_DoDof, " group='Depth of Field' ");
	TwAddVarRW(m_TweakBar, "Min Range", TW_TYPE_FLOAT, &m_MinRange, " min=-10 max=10 step=0.01 keyDecr=[ keyIncr=] group='Depth of Field' ");
	TwAddVarRW(m_TweakBar, "Max Range", TW_TYPE_FLOAT, &m_MaxRange, " min=-10 max=10 step=0.01 keyDecr=- keyIncr=+ group='Depth of Field' ");
	TwAddVarRW(m_TweakBar, "Blur Size", TW_TYPE_FLOAT, &m_Blurriness, " min=1 max=10 step=0.1 keyDecr=, keyIncr=. group='Depth of Field' ");

	TwAddVarRW(m_TweakBar, "Enable HSV", TW_TYPE_BOOL8, &m_DoHSV, " group='HSV' ");
	TwAddVarRW(m_TweakBar, "Hue", TW_TYPE_FLOAT, &m_Hue, " min=-1 max=1 step=0.01 keyDecr=[ keyIncr=] group='HSV' ");
	TwAddVarRW(m_TweakBar, "Saturation", TW_TYPE_FLOAT, &m_Saturation, " min=-1 max=1 step=0.01 keyDecr=- keyIncr=+ group='HSV' ");
	TwAddVarRW(m_TweakBar, "Value", TW_TYPE_FLOAT, &m_Value, " min=-1 max=1 step=0.01 keyDecr=, keyIncr=. group='HSV' ");

 	TwAddVarRW(m_TweakBar, "Particles", TW_TYPE_BOOL8, &m_DoParticles, " group='Particles' ");
 	TwAddVarRW(m_TweakBar, "LensFlares", TW_TYPE_BOOL8, &m_DoLensFlares, " group='LensFlares' ");

	TwAddVarRW(m_TweakBar, "Day/Night Cycle", TW_TYPE_BOOL8, &m_Sky->GetCycle(), " group='Sky' ");
	TwAddVarRW(m_TweakBar, "Cycle Speed", TW_TYPE_FLOAT, &m_Sky->GetCycleSpeed(), " min=0 max=1 step=0.01 group='Sky' ");
	TwAddVarRW(m_TweakBar, "Clouds Speed", TW_TYPE_FLOAT, &m_Sky->GetScrollSpeed(), " min=0 max=10 step=0.1 group='Sky' ");
	TwAddVarRW(m_TweakBar, "Draw Stars", TW_TYPE_BOOL8, &m_Sky->GetRenderStars(), " group='Sky' ");
	TwAddVarRW(m_TweakBar, "Draw Clouds", TW_TYPE_BOOL8, &m_Sky->GetRenderClouds(), " group='Sky' ");
	TwAddVarRW(m_TweakBar, "Moon Fullness", TW_TYPE_FLOAT, &m_Sky->GetMoonFullness(), " min=0 max=1 step=0.01 group='Sky' ");
	TwAddVarRW(m_TweakBar, "Atmosphere", TW_TYPE_FLOAT, &m_Sky->GetAtmosphere(), " min=0 max=1 step=0.01 group='Sky' ");
	TwAddVarRW(m_TweakBar, "Light Strength", TW_TYPE_FLOAT, &m_Sky->GetLightStrength(), " min=0 max=1 step=0.01 group='Sky' ");
}

Scene::~Scene(void)
{
	delete m_Camera;
	delete[] m_PreviousKeyDown;
	delete[] m_KeysPressed;
	TwTerminate();
}

void Scene::Update( double elapsedTime )
{
	HandleKeyboard( elapsedTime );

	UpdateBoat( elapsedTime );

	if(m_DoParticles)
		m_Particles->Update(elapsedTime);

	if(m_Sky->GetCycle())
		m_Sky->DayNightCycle(m_LightPos);
	
	Render( elapsedTime );
	TwDraw();
}

void Scene::HandleKeyboard( double elapsedTime )
{
	for( int i=0;i<256;i++ )
	{
		if( m_KeysDown[i] ==  true )
		{
			if( m_PreviousKeyDown[i] == false )
			{
				m_KeysPressed[i] = true;
			}
		}

		m_PreviousKeyDown[i] = m_KeysDown[i];
	}



	float moveMult = 0.3f;		// movement units per second
	float rotateMult = 25.0f;	// rotation (degrees) per second

	if( m_KeysDown[VK_SHIFT] )
	{
		moveMult *= 10.0f;
		rotateMult *= 10.0f;
	}

	if(m_KeysDown['W'])
	{
		m_Camera->MoveBF(-moveMult * (float) elapsedTime );
	}
	if(m_KeysDown['S'])
	{
		m_Camera->MoveBF(moveMult * (float) elapsedTime );
	}
	if(m_KeysDown['A'])
	{
		m_Camera->MoveLR(-moveMult * (float) elapsedTime );
	}
	if(m_KeysDown['D'])
	{
		m_Camera->MoveLR(moveMult * (float) elapsedTime );
	}
	if(m_KeysDown['R'])
	{
		m_Camera->MoveUD(moveMult * (float) elapsedTime );
	}
	if(m_KeysDown['F'])
	{
		m_Camera->MoveUD(-moveMult * (float) elapsedTime );
	}
	
	if(m_KeysDown['1'])
	{
		m_md2Model.Crossfade(AnimatedModel::AnimationType::eReady, 1);
	}
	if(m_KeysDown['2'])
	{
		m_md2Model.Crossfade(AnimatedModel::AnimationType::eRun, 1);
	}
	if(m_KeysDown['3'])
	{
		m_md2Model.Crossfade(AnimatedModel::AnimationType::eAttack, 1);
	}
	if(m_KeysDown['4'])
	{
		m_md2Model.Crossfade(AnimatedModel::AnimationType::eCrouchRun, 1);
	}
	if(m_KeysDown['5'])
	{
		m_md2Model.Crossfade(AnimatedModel::AnimationType::eCrouchAttack, 1);
	}
	if(m_KeysDown['6'])
	{
		m_md2Model.Crossfade(AnimatedModel::AnimationType::eCrouchHit, 1);
	}
	if(m_KeysDown['7'])
	{
		m_md2Model.Crossfade(AnimatedModel::AnimationType::eCrouchReady, 1);
	}
	if(m_KeysDown['8'])
	{
		m_md2Model.Crossfade(AnimatedModel::AnimationType::eWave, 1);
	}
	if(m_KeysDown['9'])
	{
		m_md2Model.Crossfade(AnimatedModel::AnimationType::ePoint, 1);
	}

	if(m_KeysDown[VK_LEFT])
	{
		m_Camera->AddYaw(-rotateMult * (float) elapsedTime );
	}
	if(m_KeysDown[VK_RIGHT])
	{
		m_Camera->AddYaw(rotateMult * (float) elapsedTime );
	}
	if(m_KeysDown[VK_UP])
	{
		m_Camera->AddPitch(rotateMult * (float) elapsedTime );
	}
	if(m_KeysDown[VK_DOWN])
	{
		m_Camera->AddPitch(-rotateMult * (float) elapsedTime );
	}
	if(m_KeysDown['Q'])
	{
		m_Camera->AddRoll(-rotateMult * (float) elapsedTime );
	}
	if(m_KeysDown['E'])
	{
		m_Camera->AddRoll(rotateMult * (float) elapsedTime );
	}


	if( m_KeysPressed['H'] )
	{
		m_KeysPressed['H'] = false;
		m_RenderDebugInfo = !m_RenderDebugInfo;
	}

// 	if( m_KeysPressed['I'] )
// 	{
// 		m_KeysPressed['I'] = false;
// 		sfactor++;
// 	}
// 
// 	if( m_KeysPressed['L'] )
// 	{
// 		m_KeysPressed['L'] = false;
// 		dfactor++;
// 	}
// 
// 	if( m_KeysPressed['K'] )
// 	{
// 		m_KeysPressed['K'] = false;
// 		sfactor--;
// 	}
// 
// 	if( m_KeysPressed['J'] )
// 	{
// 		m_KeysPressed['J'] = false;
// 		dfactor--;
// 	}
}


void Scene::Render( double elapsedTime )
{
	UpdateFPS();

	RenderMainScene( elapsedTime );

	RenderDebugInfo();
}

void Scene::RenderMainScene( double elapsedTime )
{
	glEnable(GL_TEXTURE_2D);

	RenderReflection(elapsedTime);
	RenderRefractionAndDepth();

 	m_FrameBuffer.SetAsCurrentTarget();
 
 	glClearColor( 0.25f, 0.89f, 0.82f, 1.0f );
 	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear Screen And Depth Buffer
 
 	glMatrixMode( GL_MODELVIEW );
 	glLoadIdentity();
 	
 	SetCameraTransform();
 	m_ClipPlane_Point = vector3( 0.0f, -1000000.0f, 0.0f );	// put the clip plane really low so it does not interfere with the drawing
 	m_ClipPlane_Normal = vector3( 0.0f, 1.0f, 0.0f );
 
 	StoreWorldToCameraTransform();
 
	vector3 campos = m_Camera->GetLocation();
	campos.y -= 0.01f;

	m_Sky->RenderSky(campos);
 	RenderIsland();
 	RenderBoat();
 	//RenderAnimatedModel( elapsedTime ); 	

	RenderWater(elapsedTime);

	if(m_DoParticles)
 		m_Particles->Render(m_Camera, blendmodes[sfactor], blendmodes[dfactor]);
  
  	if(m_DoLensFlares)
  	{
  		vector3 light = m_LightPos;// + vector3(0.5, 0.5 / ((float)m_Yres / (float)m_Xres), 0);
  		m_Lensflare->DetermineVisibility(light);
  		m_Lensflare->ComputeScreenCoordinates(light);	
  	}
 
 	m_FrameBuffer.StopRenderingToMe();
 
  	if(m_DoHSV)
  		ProcessHSV();
    	
  	if(m_DoDof)
  		ProcessDoF();
 
	ProcessFinalImage();
	
  	if(m_DoLensFlares)
  		m_Lensflare->Update();
}

void Scene::SetCameraTransform()
{
	GLfloat rotMatrix[ 16 ];
	m_Camera->GetOrientation().getMatrix( rotMatrix );
	glMultMatrixf( (GLfloat *)rotMatrix );
	glTranslatef(-m_Camera->GetLocation().x,-m_Camera->GetLocation().y,-m_Camera->GetLocation().z);
}

void Scene::InitCG()
{
	m_CGcontext = cgCreateContext();
	cgGLSetDebugMode( CG_FALSE );
	cgSetParameterSettingMode( m_CGcontext, CG_DEFERRED_PARAMETER_SETTING );
}


void Scene::PrintFPS()
{
	m_Font.Print( 2.0f, (float) m_Yres - 14.0f, "FPS: %.2f", m_CurrentFPS );
}

void Scene::UpdateFPS()
{
	m_FPSnumFrames++;

	double FPSelapsed = m_FPStimer.GetDurationInMicroSeconds();
	if( FPSelapsed > 1.0f )
	{
		m_FPStimer.StartTimer();
		m_CurrentFPS = (float) (m_FPSnumFrames/FPSelapsed);
		m_FPSnumFrames=0;
	}
}


void Scene::StoreWorldToCameraTransform()
{
	glGetFloatv( GL_MODELVIEW_MATRIX, m_WorldToCam );
}

void Scene::InitExtensions()
{
	if(!InitMultiTextureARB() )
	{
		MessageBox( NULL, "Multitexture extension not supported!", "Error", MB_ICONSTOP );
	}
	if(!InitFrameBufferObjectEXT() )
	{
		MessageBox( NULL, "Framebuffer Object extension not supported!", "Error", MB_ICONSTOP );
	}
	if(!InitOcclusionQueriesARB() )
	{
		MessageBox( NULL, "Occlusion Queries extension not supported!", "Error", MB_ICONSTOP );
	}
	if(!InitNonPowerOfTwoTexturesARB() )
	{
		MessageBox( NULL, "No non-power-two textures supported!", "Error", MB_ICONSTOP );
	}
}


void Scene::SetupWorld()
{
	m_LightColor = vector3( 1.0f, 1.0f, 1.0f );
	m_LightPos = vector3( 0.0f, 900.0f, 0.0f );
}


void Scene::LoadAndSetupIsland()
{
	m_Island.LoadFromFile( m_CGcontext, "models/island2.omd", 0.035f);
	m_Island.SetPosition( vector3( -10.0f, 0.30f, 0.0f ) );

	vector3 minExtent;
	vector3 maxExtent;
	m_Island.GetAABBWorldSpace( minExtent, maxExtent );
}



void Scene::LoadAndSetupBoat()
{
	m_Boat.LoadFromFile( m_CGcontext, "models/bigboat.omd", 100.0f );
	m_Boat.SetPosition(vector3( -20.0f, 0.0f, 0.0f ) );
}


void Scene::UpdateBoat( double elapsed )
{
	m_BobAngle += (float)elapsed;

	m_RollAngle = sin( m_BobAngle * 0.75f ) * 2.0f;
	m_PitchAngle = sin( m_BobAngle * 1.2f ) * 0.5f;

	m_Boat.SetPosition(vector3( -20.0f, sin(m_BobAngle)*0.001f, 0.0f ) );
	m_Boat.SetEulerAngles( vector3( m_PitchAngle, 0.0f, m_RollAngle ) );
}


void Scene::RenderIsland()
{
	m_Island.SetInfo( m_Camera->GetLocation(), m_LightPos, m_LightColor, m_WorldToCam, m_Ambient, m_ClipPlane_Point, m_ClipPlane_Normal );
	m_Island.Render( 0.0 );
}

void Scene::RenderBoat()
{
	m_Boat.SetInfo( m_Camera->GetLocation(), m_LightPos, m_LightColor, m_WorldToCam, m_Ambient, m_ClipPlane_Point, m_ClipPlane_Normal );
	m_Boat.Render( 0.0 );
}


void Scene::LoadAndSetupQ2Model()
{
	md2Reader md2reader;
	md2reader.Open( "models\\twilight.md2" );
	md2reader.Read( &m_md2Model );
	m_md2Model.CalcNormals();
	m_md2Model.SetPosition( vector3( -17.0f, 0.25f,  0.0f ) );
	m_md2Model.SetEulerAngles( vector3( 0.0f, -45.0f, 0.0f) );
	m_md2Model.SetAnimation(AnimatedModel::AnimationType::eRun);

	m_BunnyShader.Init( m_CGcontext, "Shaders/Phong_Texture" );

	m_BunnyShader_ModelCamProj =  cgGetNamedParameter( m_BunnyShader.GetVertexProgram(), "ModelCamProj");
	m_BunnyShader_ModelCam =  cgGetNamedParameter( m_BunnyShader.GetVertexProgram(), "ModelCam");
	m_BunnyShader_WorldCam =  cgGetNamedParameter( m_BunnyShader.GetPixelProgram(), "worldCam");

	m_BunnyShader_LightPos = cgGetNamedParameter( m_BunnyShader.GetPixelProgram(), "lightPos" );
	m_BunnyShader_LightColor = cgGetNamedParameter( m_BunnyShader.GetPixelProgram(), "lightColor" );
}


void Scene::RenderAnimatedModel( double elapsedTime )
{
	glPushMatrix();

	m_md2Model.SetTransform();

	cgGLSetStateMatrixParameter( m_BunnyShader_ModelCamProj, CG_GL_MODELVIEW_PROJECTION_MATRIX, CG_GL_MATRIX_IDENTITY );
	CheckOGLerror();
	cgGLSetStateMatrixParameter( m_BunnyShader_ModelCam, CG_GL_MODELVIEW_MATRIX, CG_GL_MATRIX_IDENTITY );
	CheckOGLerror();
	cgGLSetMatrixParameterfc( m_BunnyShader_WorldCam, m_WorldToCam );
	CheckOGLerror();
	cgGLSetParameter4f( m_BunnyShader_LightPos, m_LightPos.x, m_LightPos.y, m_LightPos.z, 0.0f );
	CheckOGLerror();
	cgGLSetParameter3f( m_BunnyShader_LightColor, m_LightColor.x, m_LightColor.y, m_LightColor.z );
	CheckOGLerror();

	m_BunnyShader.EnablePrograms();
	CheckOGLerror();
	m_md2Model.BindTexture();

	m_md2Model.Render( elapsedTime );
	CheckOGLerror();
	m_BunnyShader.DisablePrograms();
	CheckOGLerror();

	glPopMatrix();
}

void Scene::RenderDebugInfo()
{
// 	if( m_RenderDebugInfo )
// 	{
// 		PrintFPS();
// 
// 		m_Font.Print( 2.0f, (float) m_Yres - 28.0f, "Show debug info: %s (H)", m_RenderDebugInfo?"ON":"OFF" );
// 	}
}

void Scene::ProcessHSV()
{
	m_HSV->SetHue(m_Hue);
	m_HSV->SetSaturation(m_Saturation);
	m_HSV->SetValue(m_Value);

	glActiveTextureARB(GL_TEXTURE0_ARB);
	m_FrameBuffer.BindTexture(0);

	m_HSV->UpdateParameters(); 
	m_HSV->Enable();
	m_FrameBuffer.SetAsCurrentTarget();
	RenderToTexture();
	m_FrameBuffer.StopRenderingToMe();
	m_HSV->Disable(); 
}

void Scene::ProcessDoF()
{
	m_DoF->SetMinRange(m_MinRange);
	m_DoF->SetMaxRange(m_MaxRange);
	m_DoF->SetBlurSize(m_Blurriness);

	glActiveTextureARB(GL_TEXTURE0_ARB);
	m_FrameBuffer.BindTexture(0);

	glActiveTextureARB(GL_TEXTURE1_ARB);
	m_FrameBuffer.BindDepthBufferTexture();	

	m_DoF->UpdateParameters();

	m_DoF->Enable();
	m_FrameBuffer.SetAsCurrentTarget();
	RenderToTexture();
	m_FrameBuffer.StopRenderingToMe();
	m_DoF->Disable();
}

void Scene::RenderToTexture()
{
	glDisable( GL_DEPTH_TEST );
	glDepthMask( TRUE );

	glBegin( GL_QUADS );
	{
		glTexCoord2f( 0.0f, 1.0f );
		glVertex3f( -1.0f, 1.0f, 0.0f );

		glTexCoord2f( 1.0f, 1.0f );
		glVertex3f( 1.0f, 1.0f, 0.0f );

		glTexCoord2f( 1.0f, 0.0f );
		glVertex3f( 1.0f, -1.0f, 0.0f );

		glTexCoord2f( 0.0f, 0.0f );
		glVertex3f( -1.0f, -1.0f, 0.0f );
	}
	glEnd();

	glEnable( GL_DEPTH_TEST );
}

void Scene::ProcessFinalImage()
{
	glActiveTextureARB(GL_TEXTURE0_ARB);
	m_FrameBuffer.BindTexture(0);	

	m_ScreenSpaceQuadShader.EnablePrograms();
	RenderToTexture();
	m_ScreenSpaceQuadShader.DisablePrograms();
}

void Scene::RenderWater(float a_DT)
{
	cgGLSetStateMatrixParameter( m_WaterModelCamProj, CG_GL_MODELVIEW_PROJECTION_MATRIX, CG_GL_MATRIX_IDENTITY );
	cgGLSetStateMatrixParameter( m_WaterView, CG_GL_MODELVIEW_MATRIX, CG_GL_MATRIX_IDENTITY );

	glActiveTextureARB(GL_TEXTURE0_ARB);
	m_NormalMap.BindTexture();

	glActiveTextureARB(GL_TEXTURE1_ARB);
	m_SecondFrameBuffer.BindTexture(0);

	glActiveTextureARB(GL_TEXTURE2_ARB);
	m_RefractBuffer.BindTexture(0);

	glActiveTextureARB(GL_TEXTURE3_ARB);
	m_RefractBuffer.BindDepthBufferTexture();

	static float offset = 0;
	offset += 0.005 * a_DT;

	cgGLSetParameter4f(m_WaterLightPos, m_LightPos.x, m_LightPos.y, m_LightPos.z, 0);
	cgGLSetParameter3f(m_WaterLightCol, 1, 1, 1);
	cgGLSetParameter3f(m_WaterCamPos, m_Camera->GetLocation().x, m_Camera->GetLocation().y, m_Camera->GetLocation().z);
	cgGLSetParameter1f(m_WaterUVOffset, offset);
	
	m_WaterShader.EnablePrograms();
	glBegin( GL_QUADS );
	{		
		glTexCoord2f( 0.0f, 1000.0f );
		glNormal3f(0, 1, 0);
		glVertex3f( -1000.0f, 0.f, 1000.0f );

		glTexCoord2f( 1000.0f, 1000.0f );
		glNormal3f(0, 1, 0);
		glVertex3f( 1000.0f, 0.0f, 1000.0f );

		glTexCoord2f( 1000.0f, 0.0f );
		glNormal3f(0, 1, 0);
		glVertex3f( 1000.0f, 0.0f, -1000.0f );

		glTexCoord2f( 0.0f, 0.0f );
		glNormal3f(0, 1, 0);
		glVertex3f( -1000.0f, 0.0f, -1000.0f );
	}
	glEnd();
	m_WaterShader.DisablePrograms();

	glActiveTextureARB(GL_TEXTURE0_ARB);
}

void Scene::RenderReflection(float a_DT)
{
	m_SecondFrameBuffer.SetAsCurrentTarget();
	glClearColor( 0.25f, 0.89f, 0.82f, 1.0f );
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
	GLfloat rotMatrix[ 16 ];
	m_Camera->GetOrientation().getMatrix( rotMatrix );
	rotMatrix[4] = -rotMatrix[4];
	rotMatrix[5] = -rotMatrix[5];
	rotMatrix[6] = -rotMatrix[6];
	glMultMatrixf( (GLfloat *)rotMatrix );
	glTranslatef(-m_Camera->GetLocation().x,m_Camera->GetLocation().y,-m_Camera->GetLocation().z);
	StoreWorldToCameraTransform();

	m_ClipPlane_Point = vector3( 0.0f, 0.0f, 0.0f );
	m_ClipPlane_Normal = vector3( 0.0f, 1.0f, 0.0f );

	vector3 campos = m_Camera->GetLocation();
	campos.y = -campos.y;

	m_Sky->RenderSky(campos);
	RenderIsland();
	RenderBoat();
	//RenderAnimatedModel( a_DT );
	
	if(m_DoParticles)
		m_Particles->Render(m_Camera, blendmodes[sfactor], blendmodes[dfactor]);

	m_SecondFrameBuffer.StopRenderingToMe();
	glActiveTextureARB(GL_TEXTURE0_ARB);
}

void Scene::RenderRefractionAndDepth()
{
	m_RefractBuffer.SetAsCurrentTarget();
	glClearColor( 0.25f, 0.89f, 0.82f, 1.0f );
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();

	SetCameraTransform();

	m_ClipPlane_Point = vector3( 0.0f, 0.0f, 0.0f );
	m_ClipPlane_Normal = vector3( 0.0f, -1.0f, 0.0f );

	StoreWorldToCameraTransform();

	RenderIsland();
	RenderBoat();

	m_RefractBuffer.StopRenderingToMe();
	glActiveTextureARB(GL_TEXTURE0_ARB);
}
