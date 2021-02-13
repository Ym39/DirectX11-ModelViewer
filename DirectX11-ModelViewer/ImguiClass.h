#pragma once
#include<Windows.h>
#include "d3dclass.h"
#include "Imgui/imgui.h"
#include "Imgui/imgui_impl_win32.h"
#include "Imgui/imgui_impl_dx11.h"
#include"FileBrowser.h"
#include"TransformInspector.h"
#include"MainInspector.h"

class ImguiClass
{
public:
	ImguiClass();
	~ImguiClass() = default;

	void Initialize(HWND hwnd, ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	void Render(bool* loadFbx, std::string& filePath , Transform& transform);

private:
	FileBrowser mFileBrowser;
	TransformInspector mTransformInspector;
	MainInspector mMain;

	bool mActiveModelBrowser;
};

