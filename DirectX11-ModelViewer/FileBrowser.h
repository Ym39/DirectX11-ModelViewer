#pragma once
#include "Imgui/imgui.h"
#include "Imgui/imgui_impl_win32.h"
#include "Imgui/imgui_impl_dx11.h"
#include <string>
#include <filesystem>
#include <stack>
#include <string.h>
#include <stdio.h>
#include <string>
namespace fs = std::filesystem;

class FileBrowser
{
public:
	FileBrowser();
	~FileBrowser() = default;

	bool Render(bool* loadFbx, std::string& filePath);

private:
	fs::path mRootPath;
	fs::path mCurrentPath;
	std::vector<std::string> mCurrentDirectoryFileNames;
	std::vector<fs::path> mPopup;

	std::string mSelectFilePath;
	char* mSeletFilePathCstr;

	int mCurrentItemNumber;
	bool mIsLatest;
};

