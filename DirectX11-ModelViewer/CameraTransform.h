#pragma once
#include "Imgui/imgui.h"
#include "Imgui/imgui_impl_win32.h"
#include "Imgui/imgui_impl_dx11.h"
#include"Camera.h"
#include <limits>

class CameraTransformUI
{
public:
	CameraTransformUI() = default;
	~CameraTransformUI() = default;

	void Render(Camera& camera);
};

