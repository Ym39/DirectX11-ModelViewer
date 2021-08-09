#include "FBXLoader.h"
#include"AssetClass.h"

FBXLoader::FBXLoader()
{
    mFbxManager = FbxManager::Create();
    mIos = FbxIOSettings::Create(mFbxManager, IOSROOT);
    mFbxManager->SetIOSettings(mIos);
}

FBXLoader::~FBXLoader()
{

}

Mesh* FBXLoader::LoadFbx(char* fbxFilename)
{
    mImporter = FbxImporter::Create(mFbxManager,"");
    bool status = mImporter->Initialize(fbxFilename, -1,mFbxManager->GetIOSettings());
    if (status == false)
    {
        return nullptr;
    }

	mFbxScene = FbxScene::Create(mFbxManager,"scene");
    mImporter->Import(mFbxScene);
    mImporter->Destroy();

    FbxNode* rootNode = mFbxScene->GetRootNode();

	FbxAxisSystem sceneAxisSystem = mFbxScene->GetGlobalSettings().GetAxisSystem();
	FbxAxisSystem::DirectX.ConvertScene(mFbxScene);

	FbxGeometryConverter geometryConverter(mFbxManager);
	geometryConverter.Triangulate(mFbxScene, true);

	Mesh* finalMesh = new Mesh;

	mSkeleton = new Skeleton;
	ProcessSkeletonHierarchy(rootNode);
	/*if (mSkeleton->joints.empty() == false)
	{
	    finalMesh->SetSkeleton(mSkeleton);
	}*/

	//LoadNode(rootNode);

	//CalculateTangentAndBinormal();

	finalMesh->SetAnimationLength(mAnimationLength);
	finalMesh->SetMeshData(vertices,indices);

	if (mSkeleton->joints.empty() == false)
	{
		finalMesh->SetSkeleton(mSkeleton);
	}

	SkinnedMeshData saveMeshData;
	//saveMeshData = vertices;
	//saveMeshData = indices;
	saveMeshData = *mSkeleton;

	//ofstream out; //쓰기 스트림 생성
	//out.open("Character.SM", ios_base::binary); //바이너리 모드로 파일을 열었습니다.
	//boost::archive::binary_oarchive out_archive(out); //연 스트림을 넘겨주어서 직렬화객체 초기화
	//out_archive << saveMeshData; //쓰기
	//out.close();

 	vertices.clear();
	indices.clear();

	mFbxScene->Destroy();
    
	return finalMesh;
}

void FBXLoader::LoadFbxFile(const filesystem::path& fbxFilePath)
{
	mImporter = FbxImporter::Create(mFbxManager, "");

	bool status = mImporter->Initialize(fbxFilePath.string().c_str(), -1, mFbxManager->GetIOSettings());
	if (status == false)
	{
		return;
	}

	mFbxScene = FbxScene::Create(mFbxManager, "scene");
	mImporter->Import(mFbxScene);
	mImporter->Destroy();

	FbxNode* rootNode = mFbxScene->GetRootNode();

	FbxAxisSystem sceneAxisSystem = mFbxScene->GetGlobalSettings().GetAxisSystem();
	FbxAxisSystem::DirectX.ConvertScene(mFbxScene);

	FbxGeometryConverter geometryConverter(mFbxManager);
	geometryConverter.Triangulate(mFbxScene, true);


	mSkeleton = new Skeleton;
	ProcessSkeletonHierarchy(rootNode);
	/*if (mSkeleton->joints.empty() == false)
	{
		finalMesh->SetSkeleton(mSkeleton);
	}*/

	SkinnedMeshData saveMeshData;

	LoadNode(rootNode, saveMeshData);

	saveMeshData = *mSkeleton;

	std::string modelFileName = fbxFilePath.stem().string();
	modelFileName += ".SM";
	std::string modelPathString = AssetClass::GetModelPath().string();
	modelPathString += "\\";
	modelPathString += modelFileName;


	ofstream out; //쓰기 스트림 생성
	out.open(modelPathString.c_str(), ios_base::binary); //바이너리 모드로 파일을 열었습니다.
	boost::archive::binary_oarchive out_archive(out); //연 스트림을 넘겨주어서 직렬화객체 초기화
	out_archive << saveMeshData; //쓰기
	out.close();

	vertices.clear();
	indices.clear();

	mFbxScene->Destroy();

	AssetClass::Update();
}

void FBXLoader::LoadAnimation(char* fbxFilename)
{
	mImporter = FbxImporter::Create(mFbxManager, "");
	bool status = mImporter->Initialize(fbxFilename, -1, mFbxManager->GetIOSettings());
	if (status == false)
	{
		return;
	}

	mFbxScene = FbxScene::Create(mFbxManager, "scene");
	mImporter->Import(mFbxScene);
	mImporter->Destroy();

	FbxNode* rootNode = mFbxScene->GetRootNode();

	FbxAxisSystem sceneAxisSystem = mFbxScene->GetGlobalSettings().GetAxisSystem();
	FbxAxisSystem::DirectX.ConvertScene(mFbxScene);

	FbxGeometryConverter geometryConverter(mFbxManager);
	geometryConverter.Triangulate(mFbxScene, true);

	mSkeleton = new Skeleton;
	ProcessSkeletonHierarchy(rootNode);
	
	if (mSkeleton->joints.empty() == true)
		return;

	mSaveAnimation = new SaveAnimationData;
	mSaveAnimation->keyFrames.resize(mSkeleton->joints.size());

	LoadNodeForAnimation(rootNode);

	string fileName = fbxFilename;
	fileName += ".Animation";

	ofstream out;
	out.open("Test.Animation", ios_base::binary); //바이너리 모드로 파일을 열었습니다.
	boost::archive::binary_oarchive out_archive(out); //연 스트림을 넘겨주어서 직렬화객체 초기화
	out_archive << *mSaveAnimation; //쓰기
	out.close();

	delete mSaveAnimation;
	mSaveAnimation = nullptr;

	mFbxScene->Destroy();
}


void FBXLoader::ProcessSkeletonHierarchy(FbxNode* inRootNode)
{
	for (int childIndex = 0; childIndex < inRootNode->GetChildCount(); ++childIndex)
	{
		FbxNode* currNode = inRootNode->GetChild(childIndex);
		ProcessSkeletonHierarchyRecursively(currNode, 0, 0, -1);
	}
}

void FBXLoader::ProcessSkeletonHierarchyRecursively(FbxNode* inNode, int inDepth, int myIndex, int inParentIndex)
{
	if (inNode->GetNodeAttribute() && inNode->GetNodeAttribute()->GetAttributeType() && inNode->GetNodeAttribute()->GetAttributeType() == FbxNodeAttribute::eSkeleton)
	{
		Joint currJoint;
		currJoint.parentIndex = inParentIndex;
		currJoint.name = inNode->GetName();
		mSkeleton->joints.push_back(currJoint);
	}
	for (int i = 0; i < inNode->GetChildCount(); i++)
	{
		ProcessSkeletonHierarchyRecursively(inNode->GetChild(i), inDepth + 1, mSkeleton->joints.size(), myIndex);
	}
}

void FBXLoader::LoadNode(FbxNode* node ,SkinnedMeshData& meshData)
{	
	FbxNodeAttribute* nodeAttribute = node->GetNodeAttribute();

	if (nodeAttribute != nullptr)
	{
		if (nodeAttribute->GetAttributeType() == FbxNodeAttribute::eMesh)
		{
			string name = node->GetMesh()->GetName();
			FbxMesh* mesh = node->GetMesh();

			std::vector<XMFLOAT3> positions;
			ProcessControlPoints(mesh, positions);
			if (mSkeleton->joints.empty()==false)
			{
				ProcessJointsAndAnmations(node);
			}
			SaveMesh saveMesh;
			saveMesh.submeshs.resize(node->GetMaterialCount() == 0? 1: node->GetMaterialCount());

			ProcessMesh(mesh, saveMesh);
			//CalculateTangentAndBinormal();
			for (auto& submesh : saveMesh.submeshs)
			{
				submesh.CalculateTangentAndBinormal();
			}

			meshData.AddMesh(name, saveMesh);
			vertices.clear();
			indices.clear();
			indexMapping.clear();
		}
	}

	const int childCount = node->GetChildCount();
	for (unsigned i = 0; i < childCount; ++i)
	{
		LoadNode(node->GetChild(i),meshData);
	}
}

void FBXLoader::LoadNodeForAnimation(FbxNode* node)
{
	FbxNodeAttribute* nodeAttribute = node->GetNodeAttribute();

	if (nodeAttribute != nullptr)
	{
		if (nodeAttribute->GetAttributeType() == FbxNodeAttribute::eMesh)
		{
		    ProcessAnimations(node);
		}
	}

	const int childCount = node->GetChildCount();
	for (unsigned i = 0; i < childCount; ++i)
	{
		LoadNodeForAnimation(node->GetChild(i));
	}
}

void FBXLoader::LoadNodeJointAndAnimation(FbxNode* node)
{
	FbxNodeAttribute* nodeAttribute = node->GetNodeAttribute();

	if (nodeAttribute != nullptr)
	{
	    ProcessJointsAndAnmations(node);
	}

	const int childCount = node->GetChildCount();
	for (unsigned i = 0; i < childCount; ++i)
	{
		LoadNodeJointAndAnimation(node->GetChild(i));
	}
}

void FBXLoader::ProcessControlPoints(FbxMesh* mesh, std::vector<XMFLOAT3>& positions)
{
	unsigned int ctrlPointCount = mesh->GetControlPointsCount();
	for (unsigned int i = 0; i < ctrlPointCount; ++i)
	{
		CtrlPoint* currCtrlPoint = new CtrlPoint;
		XMFLOAT3 currPosition = XMFLOAT3(mesh->GetControlPointAt(i).mData[0], mesh->GetControlPointAt(i).mData[1], mesh->GetControlPointAt(i).mData[2]);
		currCtrlPoint->position = currPosition;
		mCtrlPoint[i] = currCtrlPoint;
	}
}

void FBXLoader::ProcessMesh(FbxMesh* mesh,SaveMesh& saveMesh)
{
	mTriangleCount = mesh->GetPolygonCount();
	int vertexCount = 0;
	vector<unordered_map<VertexType, unsigned int>> indexMapping;
	indexMapping.resize(saveMesh.submeshs.size());
	
	for (unsigned int i = 0; i < mTriangleCount; ++i)
	{
		for (unsigned int j = 0; j < 3; ++j)
		{
			int controlPointIndex = mesh->GetPolygonVertex(i, j);
			CtrlPoint* currCtrlPoint = mCtrlPoint[controlPointIndex];
			
			VertexType temp;
			temp.position = currCtrlPoint->position;
			temp.normal = ReadNormal(mesh, controlPointIndex, vertexCount);
			temp.binormal = ReadBinormals(mesh, controlPointIndex, vertexCount);
			temp.tangent = ReadTangents(mesh, controlPointIndex, vertexCount);
			temp.texture = ReadUV(mesh, controlPointIndex, mesh->GetTextureUVIndex(i, j));
			
			int elementMatCount = mesh->GetElementMaterialCount();
			int subMeshIndex = 0;
			for (int k = 0; k < elementMatCount; ++k)
			{
				FbxGeometryElementMaterial* elementMaterial = mesh->GetElementMaterial(k);
				subMeshIndex = elementMaterial->GetIndexArray().GetAt(i);
			}

			for (unsigned int i = 0; i < currCtrlPoint->blendingInfo.size(); ++i)
			{
				BlendingIndexWeightPair currBlendingInfo;
				currBlendingInfo.blendingIndex = currCtrlPoint->blendingInfo[i].blendingIndex;
				currBlendingInfo.blendingWeight = currCtrlPoint->blendingInfo[i].blendingWeight;
				temp.blendingInfo.push_back(currBlendingInfo);
			}

			auto lookup = indexMapping[subMeshIndex].find(temp);

			if (lookup != indexMapping[subMeshIndex].end())
			{
				saveMesh.submeshs[subMeshIndex].AddIndex(lookup->second);
			}
			else
			{
				unsigned int index = saveMesh.submeshs[subMeshIndex].vertices.size();
				indexMapping[subMeshIndex][temp] = index;
				saveMesh.submeshs[subMeshIndex].AddIndex(index);
				saveMesh.submeshs[subMeshIndex].AddVertex(temp);
			}
		}
	}

	for (auto itr = mCtrlPoint.begin(); itr != mCtrlPoint.end(); ++itr)
	{
		delete itr->second;
	}
	mCtrlPoint.clear();

}

void FBXLoader::CalculateTangentAndBinormal()
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

		VertexType& vertex1 = vertices[index];
		VertexType& vertex2 = vertices[index + 1];
		VertexType& vertex3 = vertices[index + 2];
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


DirectX::XMFLOAT3 FBXLoader::ReadNormal(const FbxMesh* mesh, int controlPointIndex, int vertexCounter)
{
	if (mesh->GetElementNormalCount() < 1)
		return XMFLOAT3();

	const FbxGeometryElementNormal* vertexNormal = mesh->GetElementNormal(0);
	XMFLOAT3 result;

	switch (vertexNormal->GetMappingMode())
	{
	case FbxGeometryElement::eByControlPoint:
	{
		switch (vertexNormal->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		{
			result.x = static_cast<float>(vertexNormal->GetDirectArray().GetAt(controlPointIndex).mData[0]);
			result.y = static_cast<float>(vertexNormal->GetDirectArray().GetAt(controlPointIndex).mData[1]);
			result.z = static_cast<float>(vertexNormal->GetDirectArray().GetAt(controlPointIndex).mData[2]);
		}
		break;
		case FbxGeometryElement::eIndexToDirect:
		{
			int index = vertexNormal->GetIndexArray().GetAt(controlPointIndex);
			result.x = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[0]);
			result.y = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[1]);
			result.z = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[2]);
		}
		break;
		}
	}
	break;
	case FbxGeometryElement::eByPolygonVertex:
	{
		switch (vertexNormal->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		{
			result.x = static_cast<float>(vertexNormal->GetDirectArray().GetAt(vertexCounter).mData[0]);
			result.y = static_cast<float>(vertexNormal->GetDirectArray().GetAt(vertexCounter).mData[1]);
			result.z = static_cast<float>(vertexNormal->GetDirectArray().GetAt(vertexCounter).mData[2]);
		}
		break;
		case FbxGeometryElement::eIndexToDirect:
		{
			int index = vertexNormal->GetIndexArray().GetAt(vertexCounter);
			result.x = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[0]);
			result.y = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[1]);
			result.z = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[2]);
		}
		break;
		}
	}
	break;
	}

	return result;
}

XMFLOAT2 FBXLoader::ReadUV(const FbxMesh* mesh, int controlPointIndex, int uvIndex)
{
	if (mesh->GetElementUVCount() < 1)
		return XMFLOAT2();

	const FbxGeometryElementUV* vertexUV = mesh->GetElementUV(0);
	XMFLOAT2 result;

	switch (vertexUV->GetMappingMode())
	{
	case FbxGeometryElement::eByControlPoint:
	{
		switch (vertexUV->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		{
			result.x = static_cast<float>(vertexUV->GetDirectArray().GetAt(controlPointIndex).mData[0]);
			result.y = static_cast<float>(vertexUV->GetDirectArray().GetAt(controlPointIndex).mData[1]);
		}
		break;
		case FbxGeometryElement::eIndexToDirect:
		{
			int index = vertexUV->GetIndexArray().GetAt(controlPointIndex);
			result.x = static_cast<float>(vertexUV->GetDirectArray().GetAt(index).mData[0]);
			result.y = static_cast<float>(vertexUV->GetDirectArray().GetAt(index).mData[1]);
		}
		break;
		}
	}
	break;
	case FbxGeometryElement::eByPolygonVertex:                                                             
	{
		switch (vertexUV->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		case FbxGeometryElement::eIndexToDirect:
		{
			result.x = static_cast<float>(vertexUV->GetDirectArray().GetAt(uvIndex).mData[0]);
			result.y = 1.0f - static_cast<float>(vertexUV->GetDirectArray().GetAt(uvIndex).mData[1]);
		}
		break;		
		}
	}
	break;
	}

	return result;
}

XMFLOAT3 FBXLoader::ReadBinormals(const FbxMesh* mesh, int controlPointIndex, int vertexCounter)
{
	if (mesh->GetElementBinormalCount() < 1)
		return XMFLOAT3();

	const FbxGeometryElementBinormal* vertexBinormal = mesh->GetElementBinormal(0);
	XMFLOAT3 result;

	switch (vertexBinormal->GetMappingMode())
	{
	case FbxGeometryElement::eByControlPoint:
	{
		switch (vertexBinormal->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		{
			result.x = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(controlPointIndex).mData[0]);
			result.y = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(controlPointIndex).mData[1]);
			result.z = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(controlPointIndex).mData[2]);
		}
		break;
		case FbxGeometryElement::eIndexToDirect:
		{
			int index = vertexBinormal->GetIndexArray().GetAt(controlPointIndex);
			result.x = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(index).mData[0]);
			result.y = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(index).mData[1]);
			result.z = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(index).mData[2]);
		}
		break;
		}
	}
	break;
	case FbxGeometryElement::eByPolygonVertex:
	{
		switch (vertexBinormal->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		{
			result.x = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(vertexCounter).mData[0]);
			result.y = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(vertexCounter).mData[1]);
			result.z = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(vertexCounter).mData[2]);
		}
		break;
		case FbxGeometryElement::eIndexToDirect:
		{
			int index = vertexBinormal->GetIndexArray().GetAt(vertexCounter);
			result.x = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(index).mData[0]);
			result.y = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(index).mData[1]);
			result.z = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(index).mData[2]);
		}
		break;
		}
	}
	break;
	}

	return result;
}

XMFLOAT3 FBXLoader::ReadTangents(const FbxMesh* mesh, int controlPointIndex, int vertexCounter)
{
	if (mesh->GetElementTangentCount() < 1)
		return XMFLOAT3();

	const FbxGeometryElementTangent* vertexTangent = mesh->GetElementTangent(0);
	XMFLOAT3 result;

	switch (vertexTangent->GetMappingMode())
	{
	case FbxGeometryElement::eByControlPoint:
	{
		switch (vertexTangent->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		{
			result.x = static_cast<float>(vertexTangent->GetDirectArray().GetAt(controlPointIndex).mData[0]);
			result.y = static_cast<float>(vertexTangent->GetDirectArray().GetAt(controlPointIndex).mData[1]);
			result.z = static_cast<float>(vertexTangent->GetDirectArray().GetAt(controlPointIndex).mData[2]);
		}
		break;
		case FbxGeometryElement::eIndexToDirect:
		{
			int index = vertexTangent->GetIndexArray().GetAt(controlPointIndex);
			result.x = static_cast<float>(vertexTangent->GetDirectArray().GetAt(index).mData[0]);
			result.y = static_cast<float>(vertexTangent->GetDirectArray().GetAt(index).mData[1]);
			result.z = static_cast<float>(vertexTangent->GetDirectArray().GetAt(index).mData[2]);
		}
		break;
		}
	}
	break;
	case FbxGeometryElement::eByPolygonVertex:
	{
		switch (vertexTangent->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		{
			result.x = static_cast<float>(vertexTangent->GetDirectArray().GetAt(vertexCounter).mData[0]);
			result.y = static_cast<float>(vertexTangent->GetDirectArray().GetAt(vertexCounter).mData[1]);
			result.z = static_cast<float>(vertexTangent->GetDirectArray().GetAt(vertexCounter).mData[2]);
		}
		break;
		case FbxGeometryElement::eIndexToDirect:
		{
			int index = vertexTangent->GetIndexArray().GetAt(vertexCounter);
			result.x = static_cast<float>(vertexTangent->GetDirectArray().GetAt(index).mData[0]);
			result.y = static_cast<float>(vertexTangent->GetDirectArray().GetAt(index).mData[1]);
			result.z = static_cast<float>(vertexTangent->GetDirectArray().GetAt(index).mData[2]);
		}
		break;
		}
	}
	break;
	}

	return result;
}

void FBXLoader::ProcessJointsAndAnmations(FbxNode* inNode)
{
	if (inNode == nullptr)
	{
		return;
	}

	FbxMesh* currMesh = inNode->GetMesh();
	unsigned int numOfDeformers = currMesh->GetDeformerCount();

	FbxAMatrix gemotryTransform = GetGeometryTransformation(inNode);

	for (unsigned int deformerIndex = 0; deformerIndex < numOfDeformers; ++deformerIndex)
	{
		FbxSkin* currSkin = reinterpret_cast<FbxSkin*>(currMesh->GetDeformer(deformerIndex, FbxDeformer::eSkin));
		if (!currSkin)
		{
			continue;
		}

		unsigned int numOfCluster = currSkin->GetClusterCount();
		for (unsigned int clusterIndex = 0; clusterIndex < numOfCluster; ++clusterIndex)
		{
			FbxCluster* currCluster = currSkin->GetCluster(clusterIndex);
			std::string currJointName = currCluster->GetLink()->GetName();
			unsigned int currJointIndex = FindJointIndexUsingName(currJointName);
			FbxAMatrix transformMatrix;
			FbxAMatrix transformLinkMatrix;
			FbxAMatrix globalBindposeInverseMatrix;

			currCluster->GetTransformMatrix(transformMatrix);
			currCluster->GetTransformLinkMatrix(transformLinkMatrix);
			globalBindposeInverseMatrix = transformLinkMatrix.Inverse() * transformMatrix * gemotryTransform;

			XMFLOAT4X4 boneOffset;
			for (int i = 0; i < 4; ++i)
			{
				for (int j = 0; j < 4; ++j)
				{
					boneOffset.m[i][j] = globalBindposeInverseMatrix.Get(i, j);
				}
			}

			mSkeleton->joints[currJointIndex].globalBindposeInverse = boneOffset;
			mSkeleton->joints[currJointIndex].node = currCluster->GetLink();

			unsigned int numOfIndices = currCluster->GetControlPointIndicesCount();
			for (unsigned int i = 0; i < numOfIndices; ++i)
			{
				BlendingIndexWeightPair currBlendingIndexWeightPair;
				currBlendingIndexWeightPair.blendingIndex = currJointIndex;
				currBlendingIndexWeightPair.blendingWeight = currCluster->GetControlPointWeights()[i];
				mCtrlPoint[currCluster->GetControlPointIndices()[i]]->blendingInfo.push_back(currBlendingIndexWeightPair);
			}

			//FbxAnimStack* currAnimStack = mFbxScene->GetSrcObject<FbxAnimStack>(0);
			//FbxString animStackName = currAnimStack->GetName();
			//string anmationName = animStackName.Buffer();
			//FbxTakeInfo* takeInfo = mFbxScene->GetTakeInfo(animStackName);
			//FbxTime start = takeInfo->mLocalTimeSpan.GetStart();
			//FbxTime end = takeInfo->mLocalTimeSpan.GetStop();
			//mAnimationLength = end.GetFrameCount(FbxTime::eFrames30) - start.GetFrameCount(FbxTime::eFrames30) + 1;
			//Keyframe** currAnim = &mSkeleton->joints[currJointIndex].animation;

			//int startCount = start.GetFrameCount(FbxTime::eFrames30);
			//int endCount = end.GetFrameCount(FbxTime::eFrames30);

			//for (FbxLongLong i = start.GetFrameCount(FbxTime::eFrames30); i < end.GetFrameCount(FbxTime::eFrames30); ++i)
			//{
			//	FbxTime currTime;
			//	currTime.SetFrame(i, FbxTime::eFrames30);
			//	*currAnim = new Keyframe();
			//	(*currAnim)->frameName = i;
			//	FbxAMatrix currentTransformOffset = inNode->EvaluateGlobalTransform(currTime) * gemotryTransform;
			//	FbxAMatrix fbxGlobalTransform = currentTransformOffset.Inverse() * currCluster->GetLink()->EvaluateGlobalTransform(currTime);

			//	XMFLOAT4X4 key;
			//	for (int i = 0; i < 4; ++i)
			//	{
			//		for (int j = 0; j < 4; ++j)
			//		{
			//			key.m[i][j] = fbxGlobalTransform.Get(i, j);
			//		}
			//	}

			//	(*currAnim)->globalTransfrom = key;
			//	//(*currAnim)->globalTransfrom = XMMatrixTranspose(xmGlobalTr);
			//	currAnim = &((*currAnim)->next);
			//}
		}
	}

	BlendingIndexWeightPair currBlendingIndexWeightPair;
	currBlendingIndexWeightPair.blendingIndex = 0;
	currBlendingIndexWeightPair.blendingWeight = 0;
	for (auto itr = mCtrlPoint.begin(); itr != mCtrlPoint.end(); ++itr)
	{
		for (unsigned int i = itr->second->blendingInfo.size(); i < 4; ++i)
		{
			itr->second->blendingInfo.push_back(currBlendingIndexWeightPair);
		}
	}

}

void FBXLoader::ProcessAnimations(FbxNode* inNode)
{
	if (inNode == nullptr)
	{
		return;
	}

	FbxMesh* currMesh = inNode->GetMesh();
	unsigned int numOfDeformers = currMesh->GetDeformerCount();

	FbxAMatrix gemotryTransform = GetGeometryTransformation(inNode);

	for (unsigned int deformerIndex = 0; deformerIndex < numOfDeformers; ++deformerIndex)
	{
		FbxSkin* currSkin = reinterpret_cast<FbxSkin*>(currMesh->GetDeformer(deformerIndex, FbxDeformer::eSkin));
		if (!currSkin)
		{
			continue;
		}

		unsigned int numOfCluster = currSkin->GetClusterCount();
		for (unsigned int clusterIndex = 0; clusterIndex < numOfCluster; ++clusterIndex)
		{
			FbxCluster* currCluster = currSkin->GetCluster(clusterIndex);
			std::string currJointName = currCluster->GetLink()->GetName();
			unsigned int currJointIndex = FindJointIndexUsingName(currJointName);

			FbxAnimStack* currAnimStack = mFbxScene->GetSrcObject<FbxAnimStack>(0);
			FbxString animStackName = currAnimStack->GetName();
			string anmationName = animStackName.Buffer();
			FbxTakeInfo* takeInfo = mFbxScene->GetTakeInfo(animStackName);
			FbxTime start = takeInfo->mLocalTimeSpan.GetStart();
			FbxTime end = takeInfo->mLocalTimeSpan.GetStop();
			mAnimationLength = end.GetFrameCount(FbxTime::eFrames30) - start.GetFrameCount(FbxTime::eFrames30) + 1;
			mSaveAnimation->keyFrames[currJointIndex].resize(mAnimationLength);
			mSaveAnimation->animationLength = mAnimationLength;

			for (FbxLongLong i = start.GetFrameCount(FbxTime::eFrames30); i <= end.GetFrameCount(FbxTime::eFrames30); ++i)
			{
				FbxTime currTime;
				currTime.SetFrame(i, FbxTime::eFrames30);

				//SaveKeyFrame saveKeyFrame;
				//saveKeyFrame.frameName = i;
				mSaveAnimation->keyFrames[currJointIndex][i].frameName = i;

				FbxAMatrix currentTransformOffset = inNode->EvaluateGlobalTransform(currTime) * gemotryTransform;
				FbxAMatrix fbxGlobalTransform = currentTransformOffset.Inverse() * currCluster->GetLink()->EvaluateGlobalTransform(currTime);

				Float4x4 key;
				for (int i = 0; i < 4; ++i)
				{
					for (int j = 0; j < 4; ++j)
					{
						key.m[i][j] = fbxGlobalTransform.Get(i, j);
					}
				}

				//saveKeyFrame.globalTransfrom = key;
				//mSaveAnimation->keyFrames[currJointIndex].push_back(saveKeyFrame);
				mSaveAnimation->keyFrames[currJointIndex][i].globalTransfrom = key;
			}
		}
	}

	BlendingIndexWeightPair currBlendingIndexWeightPair;
	currBlendingIndexWeightPair.blendingIndex = 0;
	currBlendingIndexWeightPair.blendingWeight = 0;
	for (auto itr = mCtrlPoint.begin(); itr != mCtrlPoint.end(); ++itr)
	{
		for (unsigned int i = itr->second->blendingInfo.size(); i < 4; ++i)
		{
			itr->second->blendingInfo.push_back(currBlendingIndexWeightPair);
		}
	}
}

unsigned int FBXLoader::FindJointIndexUsingName(const std::string& inJointName)
{
	for (unsigned int i = 0; i < mSkeleton->joints.size(); ++i)
	{
		if (mSkeleton->joints[i].name == inJointName)
		{
			return i;
		}
	}

	return 0;
}

FbxAMatrix FBXLoader::GetGeometryTransformation(FbxNode* inNode)
{
	const FbxVector4 lT = inNode->GetGeometricTranslation(FbxNode::eSourcePivot);
	const FbxVector4 lR = inNode->GetGeometricRotation(FbxNode::eSourcePivot);
	const FbxVector4 lS = inNode->GetGeometricScaling(FbxNode::eSourcePivot);

	return FbxAMatrix(lT, lR, lS);
}
