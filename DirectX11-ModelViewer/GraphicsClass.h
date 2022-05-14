#pragma once
#include "D3DClass.h"
#include "FBXLoader.h"
#include "Mesh.h"
#include "Light.h"
#include "Camera.h"
#include "Texture.h"
#include "GameObject.h"
#include "ImguiClass.h"
#include "GridClass.h"
#include "RenderTextureClass.h"
#include "ModelClass.h"
#include "ArrowModel.h"
#include "BitmapClass.h"
#include "MeshClass.h"
#include "AssetClass.h"
#include "ModelListBrowser.h"
#include "Shader.h"
#include "FrustumClass.h"
#include "ManagerInspector.h"

const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = false;
const float SCREEN_DEPTH = 10000.0f;
const float SCREEN_NEAR = 1.0f;
const int SHADOWMAP_WIDTH = 1024;
const int SHADOWMAP_HEIGHT = 1024;

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
	GraphicsClass(const GraphicsClass&) = delete;
	~GraphicsClass();

	bool Initialize(int screenWidth, int screenHeight, HWND hwnd);
	void Shutdown();
	bool Frame();
	bool Render();
	bool RenderSceneToTexture();

private:
	void AddGameObject(string key, GameObjectClass* gameObject);

private:
    D3DClass* mDirect;
	FBXLoader* mFbxLoader;
	Light* mLight;
	Camera* mCamera;
	GridClass* mGrid;
	RenderTextureClass* mRenderTexture;

	//ÇÁ·¯½ºÅÒ Å¬·¡½º
	FrustumClass* mFrustum;

	///½¦ÀÌ´õ Å¬·¡½º
	SkinnedMeshShader* mSkinnedShader;
	SpecularShaderClass* mSpecularShader;
	ColorShader* mColorShader;
	DepthShaderClass* mDepthShader;
	ShadowShaderClass* mShadowShader;
	SkinnedDepthShaderClass* mSkinnedDepthShader;
	SolidColorShader* mSolidShader;
	TextureShaderClass* mTextureShader;
	SkinnedBumpShader* mSkinnedBumpShader;
	SimpleColorShader* mSimpleColorShader;

	//2D
	BitmapClass* mMouseBitmap;

	//IMGUI
	ImguiClass* mImgui;
	ModelListBrowser modelListBrowser;
	GameObjectBrowser* gameObjectBrowser;

	//È­»ìÇ¥ ¸ðµ¨
	ArrowModel* mForwardArrowModel;
	ArrowModel* mRightArrowModel;
	ArrowModel* mUpArrowModel;

	XMMATRIX mBaseViewMatrix;

	std::unordered_map<std::string,GameObject> meshMap;
	std::string mCurrentRenderMesh;

	std::unordered_map<std::string, GameObjectClass*> mGameObejcts;
	std::vector<std::string> mGameObjectNames;
	int selectParentObject = 0;
	std::string mCurrentGameObject;
	std::string mPrevGameObject = "";

	int mScreenWidth;
	int mScreenHeight;

	PositionGizumoState mCurrentPositionGizumoState = PositionGizumoState::NONE;
};

