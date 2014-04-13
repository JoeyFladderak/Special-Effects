#pragma once

#include <Cg/cg.h>
#include <Cg/cgGL.h>

#include "vector3.h"
#include "Shader.h"
#include "Texture.h"
#include "BitmapFont.h"
#include "HighresElapsedTimer.h"
#include "OpenMDLmodel.h"
#include "FrameBufferObject.h"
#include "AnimatedModel.h"
#include "LensFlare.h"
#include "HSV.h"
#include "Blur.h"
#include "DepthOfField.h"
#include "Sky.h"
#include "AntTweakBar.h"

// forward declarations
class Camera;
class ParticleSystem;

// the Scene class itself
class Scene
{
public:
	Scene( unsigned int Xres, unsigned int Yres, bool* keys, HDC hDC, float nearPlane, float farPlane );
	virtual ~Scene(void);

	void Update( double elapsedTime );

protected:
	void HandleKeyboard( double elapsedTime );
	void Render( double elapsedTime );
	void SetCameraTransform();
	void StoreWorldToCameraTransform();
	void RenderMainScene( double elapsedTime );

	void RenderIsland();
	void RenderBoat();
	void RenderCurrentTextureWithDefaultShader();
	void RenderAnimatedModel( double elapsedTime );
	void PrintFPS();
	void UpdateFPS();
	void UpdateBoat( double elapsedTime );

	void RenderDebugInfo();

	void InitCG();
	void InitExtensions();
	void SetupWorld();
	void LoadAndSetupIsland();
	void LoadAndSetupBoat();
	void LoadAndSetupQ2Model();

	void ProcessHSV();
	void ProcessDoF();
	void RenderToTexture();
	void ProcessFinalImage();

	void RenderWater(float a_DT);
	void RenderReflection(float a_DT);
	void RenderRefractionAndDepth();

	int blendmodes[15];
	int sfactor;
	int dfactor;

	Camera *m_Camera;

	unsigned int m_Xres;
	unsigned int m_Yres;

	bool* m_KeysDown;
	bool* m_PreviousKeyDown;
	bool* m_KeysPressed;

	// the CG context
	CGcontext m_CGcontext;

	vector3 m_LightPos;
	vector3 m_LightColor;
	float m_Ambient;


	BitmapFont m_Font;

	HighResElapsedTimer m_FPStimer;
	int m_FPSnumFrames;
	float m_CurrentFPS;

	// the island
	OpenMDLmodel m_Island;

	// the boat
	OpenMDLmodel m_Boat;
	float m_BobAngle;
	float m_RollAngle;
	float m_PitchAngle;

	Shader m_WaterShader;
	CGparameter m_WaterModelCamProj;
	CGparameter m_WaterView;
	CGparameter m_WaterLightPos;
	CGparameter m_WaterLightCol;
	CGparameter m_WaterCamPos;
	CGparameter m_WaterUVOffset;
	Texture m_NormalMap;

	// the animated model
	AnimatedModel m_md2Model;
	Shader m_BunnyShader;
	CGparameter m_BunnyShader_ModelCamProj;
	CGparameter m_BunnyShader_ModelCam;
	CGparameter m_BunnyShader_WorldCam;
	CGparameter m_BunnyShader_LightPos;
	CGparameter m_BunnyShader_LightColor;
	vector3 m_BunnyRotations;

	// The framebuffer we render the scene to so we can do postprocessing on it
	FrameBufferObject m_FrameBuffer;
	Shader m_ScreenSpaceQuadShader;
	FrameBufferObject m_SecondFrameBuffer;
	FrameBufferObject m_RefractBuffer;

	// HSV Variables
	HSV* m_HSV;
	bool m_DoHSV;
	float m_Hue;
	float m_Saturation;
	float m_Value;
	
	// not used
	//Blur* m_Blur;

	// Depth of Field Variables
	DepthOfField* m_DoF;
	bool m_DoDof;
	float m_MinRange;
	float m_MaxRange;
	float m_Blurriness;

	ParticleSystem* m_Particles;
	bool m_DoParticles;

	Lensflare* m_Lensflare;
	bool m_DoLensFlares;

	//sky
	Sky* m_Sky;
	vector3 m_SunRotation;

	GLfloat m_WorldToCam[16];

	vector3 m_ClipPlane_Point;
	vector3 m_ClipPlane_Normal;

	float m_NearPlane;
	float m_FarPlane;

	bool m_RenderDebugInfo;

	TwBar* m_TweakBar;
};
