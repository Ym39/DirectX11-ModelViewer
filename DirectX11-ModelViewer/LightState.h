#pragma once
#include "Imgui/imgui.h"
#include "Imgui/imgui_impl_win32.h"
#include "Imgui/imgui_impl_dx11.h"
#include "Light.h"

class LightState
{
public:
	LightState() = default;
	~LightState() = default;

	void Render(Light* light);
};

