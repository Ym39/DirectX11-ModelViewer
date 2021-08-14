#pragma once
#include"SkinnedMeshRenderComponent.h"
#include"SkinnedBumpShader.h"

class SkinnedMeshBumpRenderComponent : public SkinnedMeshRenderComponent
{
public:
	DEFINE_COMPONENT(SkinnedMeshBumpRenderComponent, SkinnedMeshRenderComponent, false);

public:
	SkinnedMeshBumpRenderComponent():
        SkinnedMeshRenderComponent(),
		mBumpShader(nullptr)
	{
		mRendererType = eRendererType::SkinnedBumpRenderer;
	}
	~SkinnedMeshBumpRenderComponent() = default;

	bool Initalize(MeshClass* mesh, SkinnedBumpShader* shader, SkinnedDepthShaderClass* depthShader)
	{
		if (mesh == nullptr || shader == nullptr || depthShader == nullptr )
			return false;

		if (mesh->IsInitalized() == false)
			return false;

		mMesh = mesh;
		mBumpShader = shader;
		mDepthShader = depthShader;

		mUpdateBone.resize(mMesh->GetBones().size());

		for (auto& b : mUpdateBone)
		{
			b = XMMatrixTranspose(XMMatrixIdentity());
		}

		mSubObjectMats.resize(mMesh->GetSubMeshGroupCount());
		for (int i = 0; i < mSubObjectMats.size(); i++)
		{
			mSubObjectMats[i].resize(mMesh->GetSubMeshCount(i));
		}

		mBoundModel = new BoundModel(gDirect->GetDevice(), mMesh->GetBounds());

		return true;
	}

	virtual void RenderDepth(ID3D11DeviceContext* deviceContext) override
	{
		SkinnedMeshRenderComponent::RenderDepth(deviceContext);
	}

	virtual void Render(ID3D11DeviceContext* deviceContext) override
	{
		if (mMesh == nullptr)
			return;

		XMMATRIX viewMatrix, projectionMatrix;
		gMainCamera->GetViewMatrix(viewMatrix);
		gDirect->GetProjectionMatrix(projectionMatrix);

		TransformComponent* objectTransform = mOwnerGameObject->GetComponent<TransformComponent>();

		for (int i = 0; i < mMesh->GetSubMeshGroupCount(); i++)
		{
			for (int j = 0; j < mMesh->GetSubMeshCount(i); j++)
			{
				mMesh->Render(deviceContext, i, j);

				BumpSpacularMaterial& mat = mSubObjectMats[i][j];

				mBumpShader->Render(deviceContext, mMesh->GetSubMeshIndexCount(i,j), objectTransform->GetTransform(), viewMatrix, projectionMatrix, AssetClass::mTextureMap[mat.GetTextureKey()]->GetTexture(), AssetClass::mTextureMap[mat.GetNormalKey()]->GetTexture(), AssetClass::mTextureMap[mat.GetSpecularKey()]->GetTexture(), gMainLight->GetPosition(), gMainLight->GetDiffuseColor(), gMainLight->GetAmbientColor(), gMainCamera->GetPosition(), gMainLight->GetSpecularColor(), gMainLight->GetSpecularPower(), mUpdateBone);
			}
		}

		RenderBoundBox(deviceContext, objectTransform->GetTransform(), viewMatrix, projectionMatrix);
	}

	const auto& GetObjectMaterials() const { return mSubObjectMats; }
	void SetMaterial(int groupIndex, int submeshIndex,string textureKey, string normalKey, string specularKey)
	{
		mSubObjectMats[groupIndex][submeshIndex].SetTextureKey(textureKey);
		mSubObjectMats[groupIndex][submeshIndex].SetNormalKey(normalKey);
		mSubObjectMats[groupIndex][submeshIndex].SetSpecularKey(specularKey);
	}
public:
	virtual void Start() override {};
	virtual void Update(float deltaTime) override {};
	virtual void LateUpdate(float deltaTime) override
	{
		SkinnedMeshRenderComponent::LateUpdate(deltaTime);
	}

	virtual void Destroy() override {};
protected:
	SkinnedBumpShader* mBumpShader;
private:
	vector<vector<BumpSpacularMaterial>> mSubObjectMats;

};

DECLARE_COMPONENT(SkinnedMeshBumpRenderComponent);
