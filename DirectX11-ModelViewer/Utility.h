#pragma once
#include <directxmath.h>
#include<vector>
#include<unordered_map>
#include<fbxsdk.h>
#include"MathHelper.h"
using namespace DirectX;
using namespace std;

struct Keyframe
{
	FbxLongLong frameName;
	XMFLOAT4X4 globalTransfrom;
	Keyframe* next;

	Keyframe() :
		next(nullptr)
	{}

};

struct Joint
{
	std::string name;
	int parentIndex;
	XMFLOAT4X4 globalBindposeInverse;
	Keyframe* animation;
	FbxNode* node;

	Joint() :
		node(nullptr),
		animation(nullptr)
	{
		parentIndex = -1;
	}
};

struct BlendingIndexWeightPair
{
	unsigned int blendingIndex;
	double blendingWeight;

	BlendingIndexWeightPair() :
		blendingIndex(0),
		blendingWeight(0)
	{}
};

struct VertexType
{
	XMFLOAT3 position;
	XMFLOAT2 texture;
	XMFLOAT3 normal;
	std::vector<BlendingIndexWeightPair> blendingInfo;
	//unsigned int boneIndex[4];
	//XMFLOAT3 boneWeight;

	VertexType()
	{
		blendingInfo.reserve(4);
		/*for (int i = 0; i < 4; i++)
		{
			boneIndex[i] = 0;
		}
		boneWeight.x = 0.0f;
		boneWeight.y = 0.0f;
		boneWeight.z = 0.0f;*/
	}

	bool operator==(const VertexType& rhs) const
	{
		bool sameBlendingInfo = true;

		if (!(blendingInfo.empty() && rhs.blendingInfo.empty()))
		{
			// Each vertex should only have 4 index-weight blending info pairs
			for (unsigned int i = 0; i < 4; ++i)
			{
				if (blendingInfo[i].blendingIndex != rhs.blendingInfo[i].blendingIndex ||
					abs(blendingInfo[i].blendingWeight - rhs.blendingInfo[i].blendingWeight) > 0.001)
				{
					sameBlendingInfo = false;
					break;
				}
			}
		}

		bool result1 = MathHelper::CompareVector3WithEpsilon(position, rhs.position);
		bool result2 = MathHelper::CompareVector3WithEpsilon(normal, rhs.normal);
		bool result3 = MathHelper::CompareVector2WithEpsilon(texture, rhs.texture);

		return result1 && result2 && result3 && sameBlendingInfo;
	}
};

struct InputVertex
{
	XMFLOAT3 position;
	XMFLOAT2 texture;
	XMFLOAT3 normal;
	XMFLOAT3 weight;
	unsigned int boneIndices[4];
};

struct CtrlPoint
{
	XMFLOAT3 position;
	std::vector<BlendingIndexWeightPair> blendingInfo;

	CtrlPoint()
	{
		blendingInfo.reserve(4);
	}
};


struct Skeleton
{
	std::vector<Joint> joints;

	~Skeleton()
	{
		for (auto& joint : joints)
		{
			Keyframe* remove = joint.animation;
			while (remove)
			{
				Keyframe* temp = remove->next;
				delete remove;
				remove = temp;
			}
		}
	}
};

namespace std {

	template <>
	struct hash<VertexType>
	{
		std::size_t operator()(const VertexType& v) const
		{
			using std::size_t;
			using std::hash;

			// Compute individual hash values for first,
			// second and third and combine them using XOR
			// and bit shifting:

			return ((hash<float>()(v.position.x)
				^ (hash<float>()(v.position.y) << 1)) >> 1)
				^ (hash<float>()(v.position.z) << 1);
		}
	};

}