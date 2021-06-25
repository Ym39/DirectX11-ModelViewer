#pragma once
#include "Utility.h"
#include "Mesh.h"
#include<fbxsdk.h>
#include<fstream>
using namespace DirectX;
using namespace std;

class FBXLoader
{
public:
    FBXLoader();
    ~FBXLoader();
  
    Mesh* LoadFbx(char* fbxFilename);
    Skeleton* LoadAnimation(char* fbxFilename);
private:
    void ProcessSkeletonHierarchy(FbxNode* inRootNode);
    void ProcessSkeletonHierarchyRecursively(FbxNode* inNode, int inDepth, int myIndex, int inParentIndex);

	void LoadNode(FbxNode* node);
	void LoadNodeJointAndAnimation(FbxNode* node);

	void ProcessControlPoints(FbxMesh* mesh, std::vector<XMFLOAT3>& positions);
	void ProcessMesh(FbxMesh* mesh);

	XMFLOAT3 ReadNormal(const FbxMesh* mesh, int controlPointIndex, int vertexCounter);
	XMFLOAT2 ReadUV(const FbxMesh* mesh, int controlPointIndex, int uvIndex);

	void ProcessJointsAndAnmations(FbxNode* inNode);
	unsigned int FindJointIndexUsingName(const std::string& inJointName);
	FbxAMatrix GetGeometryTransformation(FbxNode* inNode);

private: 
    FbxManager* mFbxManager;
    FbxIOSettings* mIos;
    FbxImporter* mImporter;
    Skeleton* mSkeleton;
	FbxScene* mFbxScene;
	FbxLongLong mAnimationLength;

	vector<VertexType> vertices;
	unordered_map<VertexType, unsigned int> indexMapping;
	vector<unsigned int> indices;
    unsigned int mTriangleCount;
	unordered_map<unsigned int, CtrlPoint*> mCtrlPoint;
};

