#include "MainInspector.h"

void MainInspector::Render(bool* pActiveModelBrowser)
{
	bool active = true;
	ImGui::Begin("Main",&active, ImGuiWindowFlags_MenuBar);
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Load FBX Model")) 
			{
				*pActiveModelBrowser = true;
			}
			
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}

	ImGui::End();
}
