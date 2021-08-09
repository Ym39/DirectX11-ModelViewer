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
#include <boost\serialization\unordered_map.hpp>

using namespace DirectX;
using namespace std;


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

	void LoadXMFLOAT4X4(XMFLOAT4X4* matrix) const
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

struct SaveKeyFrame;
struct Keyframe
{
	FbxLongLong frameName;
	XMFLOAT4X4 globalTransfrom;
	Keyframe* next;

	Keyframe() :
		next(nullptr)
	{}
};

struct SaveKeyFrame
{
	int frameName;
	Float4x4 globalTransfrom;

	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar& frameName;
		ar& globalTransfrom;
	}
};

struct SaveAnimationData
{
	int animationLength;
	std::vector<std::vector<SaveKeyFrame>> keyFrames;

	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar& animationLength;
		ar& keyFrames;
	}
};

struct KeyFrameData
{
	FbxLongLong frameName;
	XMFLOAT4X4 globalTransfrom;

	KeyFrameData& operator=(const SaveKeyFrame& saveKeyFrame)
	{
		frameName = saveKeyFrame.frameName;
		saveKeyFrame.globalTransfrom.LoadXMFLOAT4X4(&globalTransfrom);
		return *this;
	}
};

struct AnimationData
{
	int animationLength;
	std::vector<std::vector<KeyFrameData>> keyFrames;

	AnimationData& operator=(const SaveAnimationData& saveAnimationData)
	{
		animationLength = saveAnimationData.animationLength;
		keyFrames.resize(saveAnimationData.keyFrames.size());
		for (int i = 0; i < keyFrames.size(); i++)
		{
			keyFrames[i].resize(saveAnimationData.keyFrames[i].size());
			for (int j = 0; j < keyFrames[i].size(); j++)
			{
				keyFrames[i][j] = saveAnimationData.keyFrames[i][j];
			}
		}
		return *this;
	}
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

struct Bone
{
	std::string name;
	int parentIndex;
	XMFLOAT4X4 globalBindposeInverse;

	Bone& operator=(const BoneData& boneData)
	{
		this->name = boneData.name;
		this->parentIndex = boneData.parentIndex;
		boneData.globalBindposeInverse.LoadXMFLOAT4X4(&this->globalBindposeInverse);

		return *this;
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
	XMFLOAT3 binormal;
	XMFLOAT3 tangent;
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
		bool result4 = MathHelper::CompareVector3WithEpsilon(binormal, rhs.binormal);
		bool resutl5 = MathHelper::CompareVector3WithEpsilon(tangent, rhs.tangent);

		return result1 && result2 && result3 && result4 && resutl5 &&sameBlendingInfo;
	}
};

struct SaveVertexType
{
	Float3 position;
	Float2 texture;
	Float3 normal;
	Float3 binormal;
	Float3 tangent;
	Float3 weight;
	unsigned int boneIndices[4] = { 0, };

	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar& position;
		ar& texture;
		ar& normal;
		ar& binormal;
		ar& tangent;
		ar& weight;
		ar& boneIndices;
	}

	SaveVertexType& operator=(const VertexType& copy)
	{
		position = copy.position;
		texture = copy.texture;
		normal = copy.normal;
		binormal = copy.binormal;
		tangent = copy.tangent;

		/*if (copy.blendingInfo.size() == 4)
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
		}*/

		if (copy.blendingInfo.size() == 0)
			return *this;

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
		return *this;
	}
}; 

struct InputVertex
{
	XMFLOAT3 position;
	XMFLOAT2 texture;
	XMFLOAT3 normal;
	XMFLOAT3 binormal;
	XMFLOAT3 tangent;
	XMFLOAT3 weight;
	unsigned int boneIndices[4] = {0,};

	InputVertex& operator=(const SaveVertexType& saveVertex)
	{
		this->position.x = saveVertex.position.x;
		this->position.y = saveVertex.position.y;
		this->position.z = saveVertex.position.z;

		this->texture.x = saveVertex.texture.x;
		this->texture.y = saveVertex.texture.y;

		this->normal.x = saveVertex.normal.x;
		this->normal.y = saveVertex.normal.y;
		this->normal.z = saveVertex.normal.z;

		this->binormal.x = saveVertex.binormal.x;
		this->binormal.y = saveVertex.binormal.y;
		this->binormal.z = saveVertex.binormal.z;

		this->tangent.x = saveVertex.tangent.x;
		this->tangent.y = saveVertex.tangent.y;
		this->tangent.z = saveVertex.tangent.z;

		this->weight.x = saveVertex.weight.x;
		this->weight.y = saveVertex.weight.y;
		this->weight.z = saveVertex.weight.z;

		this->boneIndices[0] = saveVertex.boneIndices[0];
		this->boneIndices[1] = saveVertex.boneIndices[1];
		this->boneIndices[2] = saveVertex.boneIndices[2];
		this->boneIndices[3] = saveVertex.boneIndices[3];

		return *this;
	}
};

struct SaveSubMesh
{
	vector<SaveVertexType> vertices;
	vector<unsigned int> indices;

	void SetData(const vector<VertexType>& vertex, const vector<unsigned int>& index)
	{
		vertices.reserve(vertex.size());
		for (const auto& v : vertex)
		{
			SaveVertexType newVertex;
			newVertex = v;
			vertices.push_back(newVertex);
		}

		indices.resize(index.size());
		copy(index.begin(), index.end(), indices.begin());
	}

	void AddVertex(const VertexType& vertex)
	{
		SaveVertexType vert;
		vert = vertex;
		vertices.push_back(vert);
	}

	void AddIndex(const unsigned int index)
	{
		indices.push_back(index);
	}

	void CalculateTangentAndBinormal()
	{
		if (vertices.empty())
			return;

		int faceCount, index = 0;

		faceCount = vertices.size() / 3;

		for (int i = 0; i < faceCount; i++)
		{
			float vector1[3], vector2[3];
			float tuVector[2], tvVector[2];
			float den;
			float length;

			SaveVertexType& vertex1 = vertices[index];
			SaveVertexType& vertex2 = vertices[index + 1];
			SaveVertexType& vertex3 = vertices[index + 2];
			index += 3;

			vector1[0] = vertex2.position.x - vertex1.position.x;
			vector1[1] = vertex2.position.y - vertex1.position.y;
			vector1[2] = vertex2.position.z - vertex1.position.z;

			vector2[0] = vertex3.position.x - vertex1.position.x;
			vector2[1] = vertex3.position.y - vertex1.position.y;
			vector2[2] = vertex3.position.z - vertex1.position.z;

			tuVector[0] = vertex2.texture.x - vertex1.texture.x;
			tuVector[1] = vertex2.texture.y - vertex1.texture.y;

			tvVector[0] = vertex3.texture.x - vertex1.texture.x;
			tvVector[1] = vertex3.texture.y - vertex1.texture.y;

			den = 1.0f / (tuVector[0] * tvVector[1] - tuVector[1] * tvVector[0]);

			XMFLOAT3 tangent;
			tangent.x = (tvVector[1] * vector1[0] - tvVector[0] * vector2[0]) * den;
			tangent.y = (tvVector[1] * vector1[1] - tvVector[0] * vector2[1]) * den;
			tangent.z = (tvVector[1] * vector1[2] - tvVector[0] * vector2[2]) * den;

			length = sqrt((tangent.x * tangent.x) + (tangent.y * tangent.y) + (tangent.z * tangent.z));

			tangent.x = tangent.x / length;
			tangent.y = tangent.y / length;
			tangent.z = tangent.z / length;

			XMFLOAT3 binormal;
			binormal.x = (tuVector[0] * vector2[0] - tuVector[1] * vector1[0]) * den;
			binormal.y = (tuVector[0] * vector2[1] - tuVector[1] * vector1[1]) * den;
			binormal.z = (tuVector[0] * vector2[2] - tuVector[1] * vector1[2]) * den;

			length = sqrt((binormal.x * binormal.x) + (binormal.y * binormal.y) + (binormal.z * binormal.z));

			binormal.x = binormal.x / length;
			binormal.y = binormal.y / length;
			binormal.z = binormal.z / length;

			vertex1.tangent = tangent;
			vertex2.tangent = tangent;
			vertex3.tangent = tangent;

			vertex1.binormal = binormal;
			vertex2.binormal = binormal;
			vertex3.binormal = binormal;
		}
	}

	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar& vertices;
		ar& indices;
	}
};

struct SaveMesh
{
	vector<SaveSubMesh> submeshs;

	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar& submeshs;
	}
};

struct SkinnedMeshData
{
	unordered_map<std::string, SaveMesh> meshs;
	vector<BoneData> bones;

	void AddMesh(std::string key, SaveMesh& mesh)
	{
		meshs[key] = mesh;
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
		ar& meshs;
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