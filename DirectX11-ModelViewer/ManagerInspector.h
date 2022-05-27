#pragma once
#include <string>

#include "Imgui/imgui.h"
#include "Imgui/imgui_impl_win32.h"
#include "Imgui/imgui_impl_dx11.h"

#include "LightManager.h"
#include "MaterialManager.h"

class ManagerInspector
{
public:
	static void RenderRightManagerInspector();
	static void RenderMaterialEdit();

	static void RenderMaterialList();

	static void ActiveMaterialEdit(std::string key);

private :
	static bool MaterialCombo(std::string name, std::string current, std::string* selected);
	static void MaterialColorEdit(std::string name, XMFLOAT4* color);

	static string currentEditMateriaKey;

	static bool activeMaterialEdit;
};
