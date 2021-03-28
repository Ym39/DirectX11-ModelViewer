#pragma once
#include "D3DClass.h"
#include "FBXLoader.h"
#include "Mesh.h"
#include "Light.h"
#include "Camera.h"
#include "SkinnedMeshShader.h"
#include "SpecularShaderClass.h"
#include "ColorShader.h"
#include "Texture.h"
#include "GameObject.h"
#include "ImguiClass.h"
#include "GridClass.h"
#include "RenderTextureClass.h"
#include "DepthShaderClass.h"
#include "ShadowShaderClass.h"
#include "ModelClass.h"

const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = false;
const float SCREEN_DEPTH = 10000.0f;
const float SCREEN_NEAR = 1.0f;
const int SHADOWMAP_WIDTH = 1024;
const int SHADOWMAP_HEIGHT = 1024;

class GraphicsClass
{
public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass&);
	~GraphicsClass();

	bool Initialize(int screenWidth, int screenHeight, HWND hwnd);
	void Shutdown();
	bool Frame();
	bool Render();
	bool RenderSceneToTexture();

private:
    D3DClass* mDirect;
	FBXLoader* mFbxLoader;
	Mesh* mMesh;
	Light* mLight;
	Camera* mCamera;
	GridClass* mGrid;
	SkinnedMeshShader* mShader;
	SpecularShaderClass* mSpecularShader;
	ColorShader* mColorShader;
	Texture* mTexture;
	GameObject* mObject;
	ImguiClass* mImgui;
	RenderTextureClass* mRenderTexture;
	DepthShaderClass* mDepthShader;
	ShadowShaderClass* mShadowShader;
	ModelClass* mGroundModel;
	Mesh* mGroundMesh;
	Texture* mGroundTexture;

	std::unordered_map<std::string,GameObject> meshMap;
	std::string mCurrentRenderMesh;
};

