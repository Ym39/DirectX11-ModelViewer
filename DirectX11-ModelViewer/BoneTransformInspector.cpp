#include "BoneTransformInspector.h"

void BoneTransformInspector::Render(GameObject* renderObj)
{
	if (renderObj != nullptr)
	{
		XMFLOAT4X4 transform;
		XMStoreFloat4x4(&transform,renderObj->GetBoneTransform()[mCurrentBoneIndex]);
		const Skeleton& skeleton = renderObj->GetSkeleton();

		float position[3] = { transform._14,transform._24,transform._34 };


		ImGui::Begin("BoneTransform");

		if (ImGui::Button("Next"))
		{
			mCurrentBoneIndex++;
			if (mCurrentBoneIndex >= skeleton.joints.size())
				mCurrentBoneIndex = 0;
		}
		if (ImGui::Button("Prev"))
		{
			mCurrentBoneIndex--;
			if (mCurrentBoneIndex < 0)
			{
				mCurrentBoneIndex = skeleton.joints.size() - 1;
			}
		}

		ImGui::Text(skeleton.joints[mCurrentBoneIndex].name.c_str());

		ImGui::InputFloat3("Position", position);

		ImGui::End();
	}
}
