#pragma once
#include "D3DClass.h"
#include "FBXLoader.h"
#include "Mesh.h"
#include "Light.h"
#include "Camera.h"
#include "SkinnedMeshShader.h"
#include "Texture.h"
#include "GameObject.h"
#include "ImguiClass.h"

const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = false;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

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

private:
    D3DClass* mDirect;
	FBXLoader* mFbxLoader;
	Mesh* mMesh;
	Light* mLight;
	Camera* mCamera;
	SkinnedMeshShader* mShader;
	Texture* mTexture;
	GameObject* mObject;
	ImguiClass* mImgui;

	std::unordered_map<std::string,GameObject> meshMap;
	std::string mCurrentRenderMesh;
};

