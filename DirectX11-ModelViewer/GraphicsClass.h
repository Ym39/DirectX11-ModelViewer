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
#include "SkinnedDepthShaderClass.h"
#include "SolidColorShader.h"
#include "ArrowModel.h"
#include "TextureShaderClass.h"
#include "BitmapClass.h"
#include "TempMesh.h"
#include "MeshClass.h"
#include "AssetClass.h"
#include "ModelListBrowser.h"
#include "SkinnedBumpShader.h"

const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = false;
const float SCREEN_DEPTH = 10000.0f;
const float SCREEN_NEAR = 1.0f;
const int SHADOWMAP_WIDTH = 1024;
const int SHADOWMAP_HEIGHT = 1024;

//Camera* gMainCamera = nullptr;
//Light* gMainLight = nullptr;
//D3DClass* gDirect = nullptr;

enum class PositionGizumoState
{
	NONE, FORWARD, RIGHT, UP
};

class GameObjectClass;
class GameObjectBrowser;

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
	void TestIntersection(int mouseX, int mouseY, XMFLOAT3 position);

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
	SkinnedDepthShaderClass* mSkinnedDepthShader;
	ModelClass* mGroundModel;
	Mesh* mGroundMesh;
	Texture* mGroundTexture;
	SolidColorShader* mSolidShader;
	ArrowModel* mForwardArrowModel;
	ArrowModel* mRightArrowModel;
	ArrowModel* mUpArrowModel;
	BitmapClass* mMouseBitmap = nullptr;
	TextureShaderClass* mTextureShader = nullptr;
	SkinnedBumpShader* mSkinnedBumpShader = nullptr;

	ModelListBrowser modelListBrowser;
	GameObjectBrowser* gameObjectBrowser;

	Texture* mCharacterTexture;
	MeshClass* mCharacterMesh;
	GameObjectClass* mGameObject;
	AnimationData* anim;
	//TempMesh* mTempMesh = nullptr;
	//vector<XMMATRIX> dummyBone;

	XMMATRIX mBaseViewMatrix;

	XMMATRIX floorWorld;

	std::unordered_map<std::string,GameObject> meshMap;
	std::string mCurrentRenderMesh;

	std::unordered_map<std::string, GameObjectClass*> mGameObejcts;

	int mScreenWidth;
	int mScreenHeight;

	PositionGizumoState mCurrentPositionGizumoState = PositionGizumoState::NONE;
};

