#include "ManagerInspector.h"

void ManagerInspector::RenderRightManagerInspector()
{
	bool active = true;
	ImGui::Begin("LightInspector", &active, ImGuiWindowFlags_None);

	for (int i = 0; i < NUM_OF_LIGHTS; i++)
	{
		std::string lightName = i == 0 ? "DirectionalLight_" : "Light_";
		lightName += std::to_string(i);

		LightSturct* currentLight = LightManager::Instance().GetLight(i);

		if (currentLight == nullptr)
		{
			continue;
		}

		if(ImGui::CollapsingHeader(lightName.c_str(), ImGuiTreeNodeFlags_Framed))
		{
			if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed))
			{
				float position[3] = { currentLight->position.x, currentLight->position.y, currentLight->position.z };
				float direction[3] = { currentLight->direction.x, currentLight->direction.y, currentLight->direction.z };
				float color[4] = { currentLight->color.x, currentLight->color.y, currentLight->color.z, currentLight->color.w };

				if (currentLight->lightType != LightType::Directional)
				{
					ImGui::InputFloat3("Position", position);
					currentLight->position.x = position[0];
					currentLight->position.y = position[1];
					currentLight->position.z = position[2];
					currentLight->position.w = 1.0f;
				}
				else
				{
					ImGui::InputFloat3("Direction", direction);
					currentLight->direction.x = direction[0];
					currentLight->direction.y = direction[1];
					currentLight->direction.z = direction[2];
					currentLight->direction.w = 0.0f;
				}

				ImGui::ColorPicker4("Color", color);
				currentLight->color.x = color[0];
				currentLight->color.y = color[1];
				currentLight->color.z = color[2];
				currentLight->color.w = color[3];

				ImGui::Checkbox("Enabled", &currentLight->enabled);

				ImGui::Checkbox("Selected", &currentLight->selected);

				const char* items[] = { "Point", "Spot", "Directional" };
				const char* currentItem = i == 0 ? "Directional" : items[(int)currentLight->lightType];

				if (ImGui::BeginCombo("LightType", currentItem, ImGuiComboFlags_NoArrowButton))
				{
					for (int n = 0; n < IM_ARRAYSIZE(items); n++)
					{
						bool isSeleted = (currentItem == items[n]);
						if (ImGui::Selectable(items[n], isSeleted))
							currentItem = items[n];
						if (isSeleted == true)
							ImGui::SetItemDefaultFocus();
					}
					ImGui::EndCombo();
				}
				int selectedLightType;
				for (selectedLightType = 0; selectedLightType < 3; selectedLightType++)
				{
					if (items[selectedLightType] == currentItem) break;
				}

				if (i == 0) selectedLightType = 2;

				currentLight->lightType = (LightType)selectedLightType;
			}
		}
	}

	ImGui::End();
}
