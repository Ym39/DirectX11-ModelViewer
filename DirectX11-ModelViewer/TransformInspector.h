#pragma once
#include "Imgui/imgui.h"
#include "Imgui/imgui_impl_win32.h"
#include "Imgui/imgui_impl_dx11.h"
#include "Transform.h"

class TransformInspector
{
public:
	TransformInspector() = default;
	~TransformInspector() = default;

	void Render(Transform& transform);
};

