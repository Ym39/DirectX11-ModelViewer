#pragma once
#include <directxmath.h>
#include<vector>
#include<unordered_map>
#include<fbxsdk.h>
#include"MathHelper.h"

#include <boost\serialization\serialization.hpp>
#include <boost\archive\binary_oarchive.hpp>
#include <boost\archive\binary_iarchive.hpp>
#include <boost\serialization\vector.hpp>
#include <boost\serialization\string.hpp>

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

struct Skeleton
{
	std::vector<Joint> joints;

	Skeleton() = default;
	Skeleton(const Skeleton& copy)
	{
		joints.resize(copy.joints.size());
		std::copy(copy.joints.begin(), copy.joints.end(), joints.begin());
	}

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

struct Float4x4
{
	union
	{
		struct
		{
			float _11, _12, _13, _14;
			float _21, _22, _23, _24;
			float _31, _32, _33, _34;
			float _41, _42, _43, _44;
		};
		float m[4][4];
	};

	Float4x4& operator=(const XMFLOAT4X4& matrix)
	{
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				m[i][j] = matrix.m[i][j];
			}
		}

		return *this;
	}

	void LoadXMFLOAT4X4(XMFLOAT4X4* matrix)
	{
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				matrix->m[i][j] = m[i][j];
			}
		}
	}

	float operator() (size_t Row, size_t Column) const { return m[Row][Column]; }

	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar& m;
	}
};

struct BoneData
{	
	std::string name;
	int parentIndex;
	Float4x4 globalBindposeInverse;

	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar& name;
		ar& parentIndex;
		ar& globalBindposeInverse;
	}
};

struct BlendingIndexWeightPair
{
	unsigned int blendingIndex;
	double blendingWeight;

	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar& blendingIndex;
		ar& blendingWeight;
	}

	BlendingIndexWeightPair() :
		blendingIndex(0),
		blendingWeight(0)
	{}
};

struct Float3
{
	float x;
	float y;
	float z;

	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar& x;
		ar& y;
		ar& z;
	}

	Float3& operator=(const XMFLOAT3& xmFloat3)
	{
		x = xmFloat3.x;
		y = xmFloat3.y;
		z = xmFloat3.z;

		return *this;
	}
};

struct Float2
{
	float x;
	float y;

	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar& x;
		ar& y;
	}

	Float2& operator=(const XMFLOAT2& xmFloat2)
	{
		x = xmFloat2.x;
		y = xmFloat2.y;

		return *this;
	}
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

		if (blendingInfo.size() != rhs.blendingInfo.size())
		{
			return false;
		}

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
	unsigned int boneIndices[4] = {0,};

	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar& position;
		ar& texture;
		ar& normal;
		ar& weight;
		ar& boneIndices;
	}
};

struct SaveVertexType
{
	Float3 position;
	Float2 texture;
	Float3 normal;
	Float3 weight;
	unsigned int boneIndices[4] = { 0, };

	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar& position;
		ar& texture;
		ar& normal;
		ar& weight;
		ar& boneIndices;
	}

	SaveVertexType& operator=(const VertexType& copy)
	{
		position = copy.position;
		texture = copy.texture;
		normal = copy.normal;

		if (copy.blendingInfo.size() == 4)
		{
			for (int i = 0; i < 4; i++)
			{
				boneIndices[i] = copy.blendingInfo[i].blendingIndex;
				switch (i)
				{
				case 0:
					weight.x = copy.blendingInfo[i].blendingWeight;
					break;
				case 1:
					weight.y = copy.blendingInfo[i].blendingWeight;
					break;
				case 2:
					weight.z = copy.blendingInfo[i].blendingWeight;
					break;
				}
			}
		}
		return *this;
	}
};

struct SkinnedMeshData
{
	vector<SaveVertexType> vertices;
	vector<unsigned int> indices;
	vector<BoneData> bones;

	SkinnedMeshData& operator=(const vector<VertexType>& vertex)
	{
		/*vertices.resize(vertex.size());
		for (int i = 0; i < vertices.size(); i++)
		{
			vertices[i].position = vertex[i].position;
			vertices[i].normal = vertex[i].normal;
			vertices[i].texture = vertex[i].texture;

			for (int j = 0; j < vertex[i].blendingInfo.size(); ++j)
			{
				vertices[i].boneIndices[j] = vertex[i].blendingInfo[j].blendingIndex;
				switch (j)
				{
				case 0:
					vertices[i].weight.x = vertex[i].blendingInfo[j].blendingWeight;
					break;
				case 1:
					vertices[i].weight.y = vertex[i].blendingInfo[j].blendingWeight;
					break;
				case 2:
					vertices[i].weight.z = vertex[i].blendingInfo[j].blendingWeight;
					break;
				}
			}
		}*/
		vertices.reserve(vertex.size());
		for (const auto& v : vertex)
		{
			SaveVertexType newVertex;
			newVertex = v;
			vertices.push_back(newVertex);
		}

		return *this;
	}

	SkinnedMeshData& operator=(const vector<unsigned int>& index)
	{
		indices.resize(index.size());
		copy(index.begin(), index.end(), indices.begin());
		return *this;
	}

	SkinnedMeshData& operator=(const Skeleton& skeleton)
	{
		bones.reserve(skeleton.joints.size());

		for (const auto& joint : skeleton.joints)
		{
			BoneData boneData;
			boneData.name = joint.name;
			boneData.parentIndex = joint.parentIndex;
			boneData.globalBindposeInverse = joint.globalBindposeInverse;
			bones.push_back(boneData);
		}

		return *this;
	}

	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar& vertices;
		ar& indices;
		ar& bones;
	}
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