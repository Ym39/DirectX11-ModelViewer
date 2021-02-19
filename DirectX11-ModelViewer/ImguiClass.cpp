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

void ImguiClass::Render(bool* loadFbx, fs::path& filePath,Transform& transform, std::unordered_map<std::string, GameObject>& meshMap, string& renderKey)
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	mMain.Render(&mActiveModelBrowser,meshMap,renderKey);

    mFileBrowser.Render(loadFbx,filePath,&mActiveModelBrowser);
	
	mTransformInspector.Render(transform);

	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}
