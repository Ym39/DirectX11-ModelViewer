#include "ImguiClass.h"

ImguiClass::ImguiClass():
	mActiveModelBrowser(false)
{ 

}

void ImguiClass::Initialize(HWND hwnd, ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplDX11_Init(device, deviceContext);
	ImGui::StyleColorsDark();
}

void ImguiClass::Render(bool* loadFbx, std::string& filePath,Transform& transform)
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	mMain.Render(&mActiveModelBrowser);

	if (mActiveModelBrowser == true)
	{
		if(mFileBrowser.Render(loadFbx,filePath) == true)
		{
			mActiveModelBrowser = false;
		}
	}
	mTransformInspector.Render(transform);

	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}
