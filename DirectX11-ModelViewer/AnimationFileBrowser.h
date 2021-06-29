#pragma once
#include "Imgui/imgui.h"
#include "Imgui/imgui_impl_win32.h"
#include "Imgui/imgui_impl_dx11.h"
#include <filesystem>
#include <stack>
#include <string.h>
#include <stdio.h>
#include <string>
namespace fs = std::filesystem;

class AnimationFileBrowser
{
public:
	AnimationFileBrowser();
	~AnimationFileBrowser() = default;

	bool Render(bool* loadFbx, fs::path & filePath, bool* active);

private:
	fs::path mRootPath;
	fs::path mCurrentPath;
	std::vector<std::string> mCurrentDirectoryFileNames;
	std::vector<fs::path> mPopup;

	std::string mSelectFilePath;
	fs::path mSelectPath;
	char* mSeletFilePathCstr;

	int mCurrentItemNumber;
	bool mIsLatest;
};

