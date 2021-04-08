#pragma once
#include "Imgui/imgui.h"
#include "Imgui/imgui_impl_win32.h"
#include "Imgui/imgui_impl_dx11.h"
#include "GameObject.h"

class BoneTransformInspector
{
public:
	BoneTransformInspector() = default;
	~BoneTransformInspector() = default;

	void Render(GameObject* renderObj);
private:
	int mCurrentBoneIndex = 0;
};

