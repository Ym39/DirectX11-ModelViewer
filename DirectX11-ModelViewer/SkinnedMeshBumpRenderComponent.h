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

		for (int i = 0; i < mMesh->GetSubMeshCount(); i++)
		{
			mSubObjectMats[mMesh->GetSubMeshName(i)] = BumpSpacularMaterial();
		}

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

		for (int i = 0; i < mMesh->GetSubMeshCount(); i++)
		{
			mMesh->Render(deviceContext, i);

			BumpSpacularMaterial& mat = mSubObjectMats[mMesh->GetSubMeshName(i)];

			mBumpShader->Render(deviceContext, mMesh->GetSubMeshIndexCount(i), objectTransform->GetTransform(), viewMatrix, projectionMatrix, AssetClass::mTextureMap[mat.GetTextureKey()]->GetTexture(), AssetClass::mTextureMap[mat.GetNormalKey()]->GetTexture(), AssetClass::mTextureMap[mat.GetSpecularKey()]->GetTexture(), gMainLight->GetPosition(), gMainLight->GetDiffuseColor(), gMainLight->GetAmbientColor(), gMainCamera->GetPosition(), gMainLight->GetSpecularColor(), gMainLight->GetSpecularPower(), mUpdateBone);
		}
	}

	const auto& GetObjectMaterials() const { return mSubObjectMats; }
	void SetMaterial(string key, string textureKey, string normalKey, string specularKey)
	{
		mSubObjectMats[key].SetTextureKey(textureKey);
		mSubObjectMats[key].SetNormalKey(normalKey);
		mSubObjectMats[key].SetSpecularKey(specularKey);
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
	unordered_map<string, BumpSpacularMaterial> mSubObjectMats;
};

DECLARE_COMPONENT(SkinnedMeshBumpRenderComponent);
