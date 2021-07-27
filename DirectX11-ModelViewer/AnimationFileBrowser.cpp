#include "AnimationFileBrowser.h"
#include "Getter.h"

namespace ImGui
{
	bool Getter2(void* list, int count, const char** outText)
	{
		std::vector<std::string>& vector = *static_cast<std::vector<std::string>*>(list);
		if (count < 0 || count > vector.size())
		{
			return false;
		}
		*outText = vector[count].c_str();

		return true;
	}
}


AnimationFileBrowser::AnimationFileBrowser():
	mIsLatest(false),
	mCurrentItemNumber(-1)
{
	mRootPath = fs::current_path();
	mCurrentPath = mRootPath;

	mCurrentDirectoryFileNames.reserve(200);
	mPopup.reserve(10);

	mSelectFilePath = "";
}

bool AnimationFileBrowser::Render(bool* loadFbx, fs::path& filePath, bool* active)
{
	if (*active == false)
	{
		return false;
	}

	if (mIsLatest == false)
	{
		mCurrentDirectoryFileNames.clear();
		for (const auto entry : std::filesystem::directory_iterator(mCurrentPath))
		{
			if (entry.is_directory() || entry.path().extension() == ".fbx")
			{
				mCurrentDirectoryFileNames.push_back(entry.path().filename().string());
			}

			//mCurrentDirectoryFileNames.push_back(entry.path().filename().string());
		}
		mIsLatest = true;
	}

	ImGui::Begin("Animation", active, ImGuiWindowFlags_MenuBar);
	/*if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("Close"))
		{
			if (ImGui::MenuItem("Close"))
			{
				*active = false;
			}
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}*/

	ImGui::Text((mCurrentPath.string()).c_str());

	if (ImGui::Button("Root"))
	{
		mCurrentPath = mRootPath;
		mPopup.clear();
		mIsLatest = false;
	}


	for (int i = 0; i < mPopup.size(); ++i)
	{
		std::string directoryName = mPopup[i].filename().string();
		ImGui::SameLine();
		if (ImGui::Button(directoryName.c_str()))
		{
			mCurrentPath = mPopup[i];
			mPopup.erase(mPopup.begin() + i + 1, mPopup.end());
			mIsLatest = false;
		}
	}


	ImGui::NewLine();
	ImGui::PushItemWidth(-1);
	bool select = ImGui::ListBox("##listbox2", &mCurrentItemNumber, ImGui::Getter2, static_cast<void*>(&mCurrentDirectoryFileNames), (int)mCurrentDirectoryFileNames.size(), 16);
	if (select == true)
	{
		fs::path selectPath = mCurrentPath / mCurrentDirectoryFileNames[mCurrentItemNumber];
		if (fs::directory_entry(selectPath).is_directory())
		{
			mCurrentPath = selectPath;
			mPopup.push_back(selectPath);
			mIsLatest = false;
		}
		else
		{
			mSelectPath = selectPath;
			mSelectFilePath = selectPath.string();
			mSeletFilePathCstr = const_cast<char*>(mSelectFilePath.c_str());

		}
	}

	ImGui::Text(mSelectFilePath.c_str());

	if (ImGui::Button("Load"))
	{
		*loadFbx = true;
		filePath = mSelectPath;
		*active = false;
	}

	ImGui::End();

	return *loadFbx;
}
