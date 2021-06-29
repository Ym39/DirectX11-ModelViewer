#pragma once
#include<Windows.h>
#include "d3dclass.h"
#include "Imgui/imgui.h"
#include "Imgui/imgui_impl_win32.h"
#include "Imgui/imgui_impl_dx11.h"
#include"FileBrowser.h"
#include"TransformInspector.h"
#include"MainInspector.h"
#include "CameraTransform.h"
#include"LightState.h"
#include "BoneTransformInspector.h"
#include "AnimationFileBrowser.h"

class ImguiClass
{
public:
	ImguiClass();
	~ImguiClass() = default;

	void Initialize(HWND hwnd, ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	void Render(bool* loadFbx, fs::path& filePath , Transform& transform, std::unordered_map<std::string, GameObject>& meshMap,string& renderKey,Camera& camera, Light* light, GameObject* currentRenderingObject, int mouseX, int mouseY, bool* loadAnim, fs::path& animfilePath);

private:
	FileBrowser mFileBrowser;
	TransformInspector mTransformInspector;
	MainInspector mMain;
	CameraTransformUI mCamerUi;
	LightState mLightState;
	BoneTransformInspector mBoneTransform;
	AnimationFileBrowser mAnimFileBrowser;

	bool mActiveModelBrowser;
	bool mActiveAnimBrowser;
};

