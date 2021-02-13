#pragma once
#include "Imgui/imgui.h"
#include "Imgui/imgui_impl_win32.h"
#include "Imgui/imgui_impl_dx11.h"

class MainInspector
{
public:
	MainInspector() = default;
	~MainInspector() = default;

	void Render(bool* pActiveModelBrowser);
};

