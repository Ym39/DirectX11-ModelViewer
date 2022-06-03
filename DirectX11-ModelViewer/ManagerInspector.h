#pragma once
#include <string>

#include "Imgui/imgui.h"
#include "Imgui/imgui_impl_win32.h"
#include "Imgui/imgui_impl_dx11.h"

#include "LightManager.h"
#include "MaterialManager.h"
#include "GameObjectManager.h"
#include "AssetClass.h"

class ManagerInspector
{
public:
	static void RenderRightManagerInspector();
	static void RenderMaterialEdit();

	static void RenderMaterialList();

	static void ActiveMaterialEdit(std::string key);

	static void RenderGameObjectList();

	static void AddGameObjectUI();

	static void RenderGameObejctEdit();
private :
	static bool TextureCombo(std::string name, std::string current, std::string* selected);
	static bool MaterialCombo(std::string name, std::string current, std::string* selected);
	static void MaterialColorEdit(std::string name, XMFLOAT4* color);

	static string currentEditMateriaKey;

	static bool activeMaterialEdit;

	static int currentGameObjectNumber;

	static bool addGameObject;

	static string currentEditGameObjectKey;

	static bool activeEditGameObject;
};
