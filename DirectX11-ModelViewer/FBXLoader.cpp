#include "FBXLoader.h"

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

	LoadNode(rootNode);

	finalMesh->SetAnimationLength(mAnimationLength);
	finalMesh->SetMeshData(vertices,indices);

	if (mSkeleton->joints.empty() == false)
	{
		finalMesh->SetSkeleton(mSkeleton);
	}

	SkinnedMeshData saveMeshData;
	saveMeshData = vertices;
	saveMeshData = indices;
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

void FBXLoader::LoadNode(FbxNode* node)
{	
	FbxNodeAttribute* nodeAttribute = node->GetNodeAttribute();

	if (nodeAttribute != nullptr)
	{
		if (nodeAttribute->GetAttributeType() == FbxNodeAttribute::eMesh)
		{
			FbxMesh* mesh = node->GetMesh();

			std::vector<XMFLOAT3> positions;
			ProcessControlPoints(mesh, positions);
			if (mSkeleton->joints.empty()==false)
			{
				ProcessJointsAndAnmations(node);
			}
			ProcessMesh(mesh);
		}
	}

	const int childCount = node->GetChildCount();
	for (unsigned i = 0; i < childCount; ++i)
	{
		LoadNode(node->GetChild(i));
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

void FBXLoader::ProcessMesh(FbxMesh* mesh)
{
	mTriangleCount = mesh->GetPolygonCount();
	int vertexCount = 0;

	for (unsigned int i = 0; i < mTriangleCount; ++i)
	{
		for (unsigned int j = 0; j < 3; ++j)
		{
			int controlPointIndex = mesh->GetPolygonVertex(i, j);
			CtrlPoint* currCtrlPoint = mCtrlPoint[controlPointIndex];

			VertexType temp;
			temp.position = currCtrlPoint->position;
			temp.normal = ReadNormal(mesh, controlPointIndex, vertexCount);
			temp.texture = ReadUV(mesh, controlPointIndex, mesh->GetTextureUVIndex(i, j));

			for (unsigned int i = 0; i < currCtrlPoint->blendingInfo.size(); ++i)
			{
				BlendingIndexWeightPair currBlendingInfo;
				currBlendingInfo.blendingIndex = currCtrlPoint->blendingInfo[i].blendingIndex;
				currBlendingInfo.blendingWeight = currCtrlPoint->blendingInfo[i].blendingWeight;
				temp.blendingInfo.push_back(currBlendingInfo);
			}

			auto lookup = indexMapping.find(temp);

			if (lookup != indexMapping.end())
			{
				indices.push_back(lookup->second);
			}
			else
			{
				unsigned int index = vertices.size();
				indexMapping[temp] = index;
				indices.push_back(index);
				vertices.push_back(temp);
			}
		}
	}

	for (auto itr = mCtrlPoint.begin(); itr != mCtrlPoint.end(); ++itr)
	{
		delete itr->second;
	}
	mCtrlPoint.clear();

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

			FbxAnimStack* currAnimStack = mFbxScene->GetSrcObject<FbxAnimStack>(0);
			FbxString animStackName = currAnimStack->GetName();
			string anmationName = animStackName.Buffer();
			FbxTakeInfo* takeInfo = mFbxScene->GetTakeInfo(animStackName);
			FbxTime start = takeInfo->mLocalTimeSpan.GetStart();
			FbxTime end = takeInfo->mLocalTimeSpan.GetStop();
			mAnimationLength = end.GetFrameCount(FbxTime::eFrames30) - start.GetFrameCount(FbxTime::eFrames30) + 1;
			Keyframe** currAnim = &mSkeleton->joints[currJointIndex].animation;

			int startCount = start.GetFrameCount(FbxTime::eFrames30);
			int endCount = end.GetFrameCount(FbxTime::eFrames30);

			for (FbxLongLong i = start.GetFrameCount(FbxTime::eFrames30); i < end.GetFrameCount(FbxTime::eFrames30); ++i)
			{
				FbxTime currTime;
				currTime.SetFrame(i, FbxTime::eFrames30);
				*currAnim = new Keyframe();
				(*currAnim)->frameName = i;
				FbxAMatrix currentTransformOffset = inNode->EvaluateGlobalTransform(currTime) * gemotryTransform;
				FbxAMatrix fbxGlobalTransform = currentTransformOffset.Inverse() * currCluster->GetLink()->EvaluateGlobalTransform(currTime);

				XMFLOAT4X4 key;
				for (int i = 0; i < 4; ++i)
				{
					for (int j = 0; j < 4; ++j)
					{
						key.m[i][j] = fbxGlobalTransform.Get(i, j);
					}
				}

				(*currAnim)->globalTransfrom = key;
				//(*currAnim)->globalTransfrom = XMMatrixTranspose(xmGlobalTr);
				currAnim = &((*currAnim)->next);
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
