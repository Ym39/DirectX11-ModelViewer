//#include "ManagerInspector.h"
//#include "AssetClass.h"
//#include "TransformComponent.h"
//#include "LightMeshRenderComponent.h"
//
//bool ManagerInspector::activeMaterialEdit = false;
//string ManagerInspector::currentEditMateriaKey = "";
//int ManagerInspector::currentGameObjectNumber = 0;
//bool ManagerInspector::addGameObject = false;
//string ManagerInspector::currentEditGameObjectKey = "";
//bool ManagerInspector::activeEditGameObject = false;
//
//bool VectorGetterManager(void* list, int count, const char** outText)
//{
//	std::vector<std::string>& vector = *static_cast<std::vector<std::string>*>(list);
//	if (count < 0 || count > vector.size())
//	{
//		return false;
//	}
//	*outText = vector[count].c_str();
//
//	return true;
//}
//
//void ManagerInspector::RenderRightManagerInspector()
//{
//	bool active = true;
//	ImGui::Begin("LightInspector", &active, ImGuiWindowFlags_None);
//
//	for (int i = 0; i < NUM_OF_LIGHTS; i++)
//	{
//		std::string lightName = i == 0 ? "DirectionalLight_" : "Light_";
//		lightName += std::to_string(i);
//
//		LightSturct* currentLight = LightManager::Instance().GetLight(i);
//
//		if (currentLight == nullptr)
//		{
//			continue;
//		}
//
//		if(ImGui::CollapsingHeader(lightName.c_str(), ImGuiTreeNodeFlags_Framed))
//		{
//			if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed))
//			{
//				float position[3] = { currentLight->position.x, currentLight->position.y, currentLight->position.z };
//				float direction[3] = { currentLight->direction.x, currentLight->direction.y, currentLight->direction.z };
//				float color[4] = { currentLight->color.x, currentLight->color.y, currentLight->color.z, currentLight->color.w };
//
//				if (currentLight->lightType != LightType::Directional)
//				{
//					ImGui::InputFloat3("Position", position);
//					currentLight->position.x = position[0];
//					currentLight->position.y = position[1];
//					currentLight->position.z = position[2];
//					currentLight->position.w = 1.0f;
//				}
//				else
//				{
//					ImGui::InputFloat3("Direction", direction);
//					currentLight->direction.x = direction[0];
//					currentLight->direction.y = direction[1];
//					currentLight->direction.z = direction[2];
//					currentLight->direction.w = 0.0f;
//				}
//
//				ImGui::ColorPicker4("Color", color);
//				currentLight->color.x = color[0];
//				currentLight->color.y = color[1];
//				currentLight->color.z = color[2];
//				currentLight->color.w = color[3];
//
//				ImGui::Checkbox("Enabled", &currentLight->enabled);
//
//				ImGui::Checkbox("Selected", &currentLight->selected);
//
//				const char* items[] = { "Point", "Spot", "Directional" };
//				const char* currentItem = i == 0 ? "Directional" : items[(int)currentLight->lightType];
//
//				if (ImGui::BeginCombo("LightType", currentItem, ImGuiComboFlags_NoArrowButton))
//				{
//					for (int n = 0; n < IM_ARRAYSIZE(items); n++)
//					{
//						bool isSeleted = (currentItem == items[n]);
//						if (ImGui::Selectable(items[n], isSeleted))
//							currentItem = items[n];
//						if (isSeleted == true)
//							ImGui::SetItemDefaultFocus();
//					}
//					ImGui::EndCombo();
//				}
//				int selectedLightType;
//				for (selectedLightType = 0; selectedLightType < 3; selectedLightType++)
//				{
//					if (items[selectedLightType] == currentItem) break;
//				}
//
//				if (i == 0) selectedLightType = 2;
//
//				currentLight->lightType = (LightType)selectedLightType;
//			}
//		}
//	}
//
//	ImGui::End();
//}
//
//void ManagerInspector::RenderMaterialEdit()
//{
//	WideMaterial* curMat = MaterialManager::Instance().GetMaterial(currentEditMateriaKey);
//
//	if (curMat == nullptr || activeMaterialEdit == false)
//	{
//		return;
//	}
//
//	ImGui::Begin("Edit Material", &activeMaterialEdit, ImGuiWindowFlags_None);
//
//	static vector<std::string> textureNames;
//
//	if (textureNames.size() != AssetClass::GetTextures().size())
//	{
//		for (const auto& texture : AssetClass::GetTextures())
//		{
//			textureNames.push_back(texture.first);
//		}
//	}
//
//	MaterialColorEdit("Global Ambient Color", &curMat->shaderUploadMaterial.globalAmbient);
//	MaterialColorEdit("Reflectance", &curMat->shaderUploadMaterial.reflectance);
//	ImGui::InputFloat("Opacity", &curMat->shaderUploadMaterial.opacity);
//	ImGui::InputFloat("SpecularPower", &curMat->shaderUploadMaterial.specularPower);
//	ImGui::InputFloat("IndexOfRefraction", &curMat->shaderUploadMaterial.indexOfRefraction);
//	ImGui::InputFloat("BumpIntensity", &curMat->shaderUploadMaterial.bumpIntensity);
//	ImGui::InputFloat("SpecularScale", &curMat->shaderUploadMaterial.specularScale);
//	ImGui::InputFloat("AlphaThreshold", &curMat->shaderUploadMaterial.alphaThreshold);
//
//	if (ImGui::CollapsingHeader("Ambient", ImGuiTreeNodeFlags_Framed))
//	{
//		ImGui::Checkbox("HasAmbient", &curMat->shaderUploadMaterial.hasAmbientTexture);
//
//		std::string selected = "";
//
//		if (TextureCombo("Ambient Texture", curMat->GetAmbientKey(), &selected) == true)
//		{
//			curMat->SetAmbientKey(selected);
//		}
//
//		ImGui::Image((void*)AssetClass::mTextureMap[curMat->GetAmbientKey()]->GetTexture(), ImVec2(128.0f, 128.0f));
//
//		MaterialColorEdit("Ambient Color", &curMat->shaderUploadMaterial.ambientColor);
//	}
//
//	if (ImGui::CollapsingHeader("Emissive", ImGuiTreeNodeFlags_Framed))
//	{
//		ImGui::Checkbox("HasEmissive", &curMat->shaderUploadMaterial.hasEmissiveTexture);
//
//		std::string selected = "";
//
//		if (TextureCombo("Emissive Texture", curMat->GetEmissiveKey(), &selected) == true)
//		{
//			curMat->SetEmissiveKey(selected);
//		}
//
//		ImGui::Image((void*)AssetClass::mTextureMap[curMat->GetEmissiveKey()]->GetTexture(), ImVec2(128.0f, 128.0f));
//
//		MaterialColorEdit("Emissive Color", &curMat->shaderUploadMaterial.emissiveColor);
//	}
//		
//	if (ImGui::CollapsingHeader("Diffuse", ImGuiTreeNodeFlags_Framed))
//	{
//		ImGui::Checkbox("HasDiffuse", &curMat->shaderUploadMaterial.hasDiffuseTexture);
//
//		std::string selected = "";
//
//		if (TextureCombo("Diffuse Texture", curMat->GetDiffuseKey(), &selected) == true)
//		{
//			curMat->SetDiffuseKey(selected);
//		}
//
//		ImGui::Image((void*)AssetClass::mTextureMap[curMat->GetDiffuseKey()]->GetTexture(), ImVec2(128.0f, 128.0f));
//
//		MaterialColorEdit("Diffuse Color", &curMat->shaderUploadMaterial.emissiveColor);
//	}
//
//	if (ImGui::CollapsingHeader("Specular", ImGuiTreeNodeFlags_Framed))
//	{
//		ImGui::Checkbox("HasSpecular", &curMat->shaderUploadMaterial.hasSpecularTexture);
//
//		std::string selected = "";
//
//		if (TextureCombo("Specular Texture", curMat->GetSpecularKey(), &selected) == true)
//		{
//			curMat->SetSpecularKey(selected);
//		}
//
//		ImGui::Image((void*)AssetClass::mTextureMap[curMat->GetSpecularKey()]->GetTexture(), ImVec2(128.0f, 128.0f));
//
//		MaterialColorEdit("Specular Color", &curMat->shaderUploadMaterial.emissiveColor);
//
//		std::string selected2 = "";
//
//		if (TextureCombo("Specular Power Texture", curMat->GetSpecularPowerKey(), &selected2) == true)
//		{
//			curMat->SetSpecularPowerKey(selected);
//		}
//
//	}
//
//	if (ImGui::CollapsingHeader("Normal", ImGuiTreeNodeFlags_Framed))
//	{
//		ImGui::Checkbox("HasNormal", &curMat->shaderUploadMaterial.hasNormalTexture);
//
//		std::string selected = "";
//
//		if (TextureCombo("Normal Texture", curMat->GetNormalKey(), &selected) == true)
//		{
//			curMat->SetNormalKey(selected);
//		}
//
//		ImGui::Image((void*)AssetClass::mTextureMap[curMat->GetNormalKey()]->GetTexture(), ImVec2(128.0f, 128.0f));
//	}
//
//	if (ImGui::CollapsingHeader("Bump", ImGuiTreeNodeFlags_Framed))
//	{
//		ImGui::Checkbox("HasBump", &curMat->shaderUploadMaterial.hasBumpTexture);
//
//		std::string selected = "";
//
//		if (TextureCombo("Bump Texture", curMat->GetBumpKey(), &selected) == true)
//		{
//			curMat->SetBumpKey(selected);
//		}
//
//		ImGui::Image((void*)AssetClass::mTextureMap[curMat->GetBumpKey()]->GetTexture(), ImVec2(128.0f, 128.0f));
//	}
//
//	if (ImGui::CollapsingHeader("Opacity", ImGuiTreeNodeFlags_Framed))
//	{
//		ImGui::Checkbox("HasOpacity", &curMat->shaderUploadMaterial.hasOpacityTexture);
//
//		std::string selected = "";
//
//		if (TextureCombo("Opacity Texture", curMat->GetOpacityKey(), &selected) == true)
//		{
//			curMat->SetOpacityKey(selected);
//		}
//
//		ImGui::Image((void*)AssetClass::mTextureMap[curMat->GetOpacityKey()]->GetTexture(), ImVec2(128.0f, 128.0f));
//	}
//
//	ImGui::End();
//}
//
//void ManagerInspector::RenderMaterialList()
//{
//	bool active = true;
//	ImGui::Begin("Material Manager", &active, ImGuiWindowFlags_None);
//
//	ImGui::ListBoxHeader("Material List");
//	for (const auto& item : MaterialManager::Instance().GetMaterialKeyList())
//	{
//		bool isSelected = false;
//		if (ImGui::Selectable(item.c_str(), &isSelected))
//		{
//		}
//
//		if (isSelected == true)
//		{
//			ActiveMaterialEdit(item);
//		}
//	}
//	ImGui::ListBoxFooter();
//
//	static char buffer[32] = { 0 };
//
//	ImGui::InputText("New Material", buffer, IM_ARRAYSIZE(buffer));
//	if (ImGui::Button("Add Material") == true)
//	{
//		MaterialManager::Instance().AddNewMaterial(buffer);
//	}
//
//	ImGui::End();
//}
//
//void ManagerInspector::ActiveMaterialEdit(std::string key)
//{
//	activeMaterialEdit = true;
//	currentEditMateriaKey = key;
//}
//
//void ManagerInspector::RenderGameObjectList()
//{
//	bool active;
//	ImGui::Begin("GameObject List", &active, ImGuiWindowFlags_MenuBar);
//	ImGui::NewLine();
//	ImGui::PushItemWidth(-1);
//
//	vector<string>& gameObjectNames = GameObjectManager::Instance().GetAllGameObjectName();
//
//	bool select = ImGui::ListBox("##listbox434", &currentGameObjectNumber, VectorGetterManager, static_cast<void*>(&gameObjectNames),(int)gameObjectNames.size(), 16);
//
//	if (ImGui::Button("Add GameObject #"))
//	{
//		addGameObject = true;
//	}
//
//	ImGui::End();
//}
//
//void ManagerInspector::AddGameObjectUI()
//{
//	if (addGameObject == false)
//	{
//		return;
//	}
//
//	ImGui::Begin("AddGameObject Browser", &addGameObject, ImGuiWindowFlags_None);
//
//	static char buffer[32] = { 0 };
//
//	ImGui::InputText("GameObjectName", buffer, IM_ARRAYSIZE(buffer));
//
//	ImGui::NewLine();
//	ImGui::PushItemWidth(-1);
//
//	vector<string> modelFileNames;
//
//	for (const auto& mesh : AssetClass::GetMeshs())
//	{
//		modelFileNames.push_back(mesh.first);
//	}
//
//	static int currentSelectedModleNumber;
//	bool selectModel = ImGui::Combo("ModelCombo", &currentSelectedModleNumber, VectorGetterManager, static_cast<void*>(&modelFileNames), modelFileNames.size(), 16);
//	string selectedModelKey = modelFileNames[currentSelectedModleNumber];
//
//	ImGui::NewLine();
//
//	if (ImGui::Button("Confirm Add GameObject"))
//	{
//		GameObjectClass* newGameObejct = GameObjectClass::Create();
//		newGameObejct->InsertComponent(new TransformComponent());
//		
//		LightMeshRenderComponenet* renderComp = new LightMeshRenderComponenet();
//		renderComp->Initalize(AssetClass::mMeshMap[selectedModelKey], ShaderManager::Instance().GetLighatMeshShader());
//
//		newGameObejct->InsertComponent(renderComp);
//
//		GameObjectManager::Instance().AddGameObject(buffer, newGameObejct);
//
//		addGameObject = false;
//	}
//
//	ImGui::End();
//}
//
//void ManagerInspector::RenderGameObejctEdit()
//{
//	if (activeEditGameObject == false)
//	{
//		return;
//	}
//
//	ImGui::Begin("Edit GameObject", &activeEditGameObject, ImGuiWindowFlags_None);
//
//	GameObjectClass* gameObject = GameObjectManager::Instance().GetGameObjcet(currentEditGameObjectKey);
//
//	if (gameObject == nullptr)
//	{
//		ImGui::End();
//		return;
//	}
//
//	TransformComponent* transform = gameObject->GetComponent<TransformComponent>();
//	if (transform != nullptr)
//	{
//		if (ImGui::CollapsingHeader("Transform_EditGameObject", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed))
//		{
//			XMFLOAT3 pos = transform->GetLocalPosition();
//			float position[3] = { pos.x,pos.y,pos.z };
//			XMFLOAT3 rot = transform->GetLocalRotation();
//			float rotation[3] = { rot.x,rot.y ,rot.z };
//			XMFLOAT3 s = transform->GetLocalScale();
//			float scale[3] = { s.x,s.y,s.z };
//
//			ImGui::InputFloat3("Position", position);
//			ImGui::InputFloat3("Rotation", rotation);
//			ImGui::InputFloat3("Scale", scale);
//
//			transform->SetLocalPosition(XMFLOAT3(position[0], position[1], position[2]));
//			transform->SetLocalRotation(XMFLOAT3(rotation[0], rotation[1], rotation[2]));
//			transform->SetLocalScale(XMFLOAT3(scale[0], scale[1], scale[2]));
//
//			/*if (transformComp->GetParent() == nullptr)
//				selectParentObject = 0;
//
//			if (ImGui::Combo("Parent", &selectParentObject, VectorGetterManager, static_cast<void*>(&mGameObjectNames), mGameObjectNames.size(), 16))
//			{
//				if (selectParentObject != 0)
//				{
//					TransformComponent* parent = mGameObejcts[mGameObjectNames[selectParentObject]]->GetComponent<TransformComponent>();
//					transformComp->SetParent(parent);
//				}
//			}*/
//		}
//	}
//
//	LightMeshRenderComponenet* render = gameObject->GetComponent<LightMeshRenderComponenet>();
//
//	if (render != nullptr)
//	{
//		auto& matNames = MaterialManager::Instance().GetMaterialKeyList();
//		if (ImGui::CollapsingHeader("Material_EditGameObject", ImGuiTreeNodeFlags_Framed))
//		{
//			auto& subObjectMats = render->GetObjectMaterialKeys();
//			for (int i = 0; i < subObjectMats.size(); i++)
//			{
//				string meshGroupLabel = "MeshGroup_";
//				string num = to_string(i);
//				meshGroupLabel += num;
//
//				if (ImGui::CollapsingHeader(meshGroupLabel.c_str(), ImGuiTreeNodeFlags_Framed))
//				{
//					for (int j = 0; j < subObjectMats[i].size(); j++)
//					{
//						string subMeshLabel = "Material_";
//						string num = to_string(j);
//						subMeshLabel += num;
//						if (ImGui::CollapsingHeader(subMeshLabel.c_str(), ImGuiTreeNodeFlags_Framed))
//						{
//							int selectTextureCount = 0;
//							for (int textIdx = 0; textIdx < matNames.size(); textIdx++)
//							{
//								if (subObjectMats[i][j] == matNames[textIdx])
//								{
//									selectTextureCount = textIdx;
//									break;
//								}
//							}
//							if (ImGui::Combo("Material Name", &selectTextureCount, VectorGetterManager, static_cast<void*>(&matNames), matNames.size(), 16))
//							{
//								render->SetMaterialKey(i, j, matNames[selectTextureCount]);
//							}
//						}
//					}
//				}
//			}
//		}
//	}
//
//	ImGui::End();
//}
//
//bool ManagerInspector::TextureCombo(std::string name, std::string current, std::string* selected)
//{
//	static vector<std::string> textureNames;
//
//	if (textureNames.size() != AssetClass::GetTextures().size())
//	{
//		for (const auto& texture : AssetClass::GetTextures())
//		{
//			textureNames.push_back(texture.first);
//		}
//	}
//	
//	bool isSeleted = false;
//	bool isChanged = false;
//
//	if (ImGui::BeginCombo(name.c_str(), current.c_str(), ImGuiComboFlags_NoArrowButton))
//	{
//		for (int n = 0; n < textureNames.size(); n++)
//		{
//			isSeleted = (current == textureNames[n]);
//			if (ImGui::Selectable(textureNames[n].c_str(), isSeleted))
//			{
//				isChanged = true;
//				current = textureNames[n];
//				*selected = current;
//			}
//			if (isSeleted == true)
//				ImGui::SetItemDefaultFocus();
//		}
//		ImGui::EndCombo();
//	}
//
//	return isChanged;
//}
//
//bool ManagerInspector::MaterialCombo(std::string name, std::string current, std::string* selected)
//{
//	static vector<std::string> materialsNames;
//
//	if (materialsNames.size() != MaterialManager::Instance().GetMaterialKeyList().size())
//	{
//		for (const auto& matNames : MaterialManager::Instance().GetMaterialKeyList())
//		{
//			materialsNames.push_back(matNames);
//		}
//	}
//
//	bool isSeleted = false;
//	bool isChanged = false;
//
//	if (ImGui::BeginCombo(name.c_str(), current.c_str(), ImGuiComboFlags_NoArrowButton))
//	{
//		for (int n = 0; n < materialsNames.size(); n++)
//		{
//			isSeleted = (current == materialsNames[n]);
//			if (ImGui::Selectable(materialsNames[n].c_str(), isSeleted))
//			{
//				isChanged = true;
//				current = materialsNames[n];
//				*selected = current;
//			}
//			if (isSeleted == true)
//				ImGui::SetItemDefaultFocus();
//		}
//		ImGui::EndCombo();
//	}
//
//	return isChanged;
//}
//
//void ManagerInspector::MaterialColorEdit(std::string name, XMFLOAT4* color)
//{
//	float color4[4] = { color->x, color->y, color->z, color->w };
//
//	ImGui::ColorEdit4(name.c_str(), color4);
//
//	(*color).x = color4[0];
//	(*color).y = color4[1];
//	(*color).z = color4[2];
//	(*color).w = color4[3];
//}
//
