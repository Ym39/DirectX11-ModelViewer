#include "ImguiClass.h"

ImguiClass::ImguiClass():
	mActiveModelBrowser(false),
	mActiveAnimBrowser(false)
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

void ImguiClass::Render(bool* loadFbx, fs::path& filePath,Transform& transform, std::unordered_map<std::string, GameObject>& meshMap, string& renderKey, Camera& camera, Light* light, GameObject* currentRenderingObject, int mouseX, int mouseY, bool* loadAnim, fs::path& animfilePath)
{
	/*ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();*/
	/*static bool active = true;
	std::string click = "false";

	ImGui::Begin("DFFF", &active, ImGuiWindowFlags_MenuBar);
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Load FBX Model"))
			{
			}

			if (ImGui::MenuItem("Load Animation"))
			{
			}
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}

	ImGui::NewLine();
	ImGui::PushItemWidth(-1);
	std::string selectString = "False";
	if (ImGui::Button("Play"))
	{
		selectString = "true";
	}
	ImGui::Text("%s", selectString.c_str());

	ImGui::End();*/

	mMain.Render(&mActiveModelBrowser,&mActiveAnimBrowser,meshMap,renderKey);

	mCamerUi.Render(camera);

    mFileBrowser.Render(loadFbx,filePath,&mActiveModelBrowser);

	mAnimFileBrowser.Render(loadAnim, animfilePath, &mActiveAnimBrowser);
	
	mTransformInspector.Render(transform);

	mLightState.Render(light);

	mBoneTransform.Render(currentRenderingObject);

	ImGui::Begin("DebugText");
	ImGui::Text(std::to_string(mouseX).c_str());
	ImGui::Text(std::to_string(mouseY).c_str());

	ImGui::End();



	//ImGui::Render();
	//ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}
