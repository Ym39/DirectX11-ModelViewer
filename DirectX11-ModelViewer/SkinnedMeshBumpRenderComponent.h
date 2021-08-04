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
		mBumpShader(nullptr),
		mBumpTexture(nullptr),
		mSpecularTexture(nullptr)
	{}
	~SkinnedMeshBumpRenderComponent() = default;

	bool Initalize(MeshClass* mesh, SkinnedBumpShader* shader, SkinnedDepthShaderClass* depthShader, Texture* texture, Texture* bumpTexture, Texture* specularTexture)
	{
		if (mesh == nullptr || shader == nullptr || texture == nullptr || depthShader == nullptr || bumpTexture == nullptr || specularTexture == nullptr)
			return false;

		if (mesh->IsInitalized() == false)
			return false;

		mMesh = mesh;
		mBumpShader = shader;
		mTexture = texture;
		mDepthShader = depthShader;
		mBumpTexture = bumpTexture;
		mSpecularTexture = specularTexture;

		mUpdateBone.resize(mMesh->GetBones().size());

		for (auto& b : mUpdateBone)
		{
			b = XMMatrixTranspose(XMMatrixIdentity());
		}

		return true;
	}

	virtual void RenderDepth(ID3D11DeviceContext* deviceContext) override
	{
		SkinnedMeshRenderComponent::RenderDepth(deviceContext);
	}

	virtual void Render(ID3D11DeviceContext* deviceContext) override
	{
		if (mMesh == nullptr || mTexture == nullptr || mBumpTexture == nullptr || mSpecularTexture == nullptr)
			return;

		XMMATRIX viewMatrix, projectionMatrix;
		gMainCamera->GetViewMatrix(viewMatrix);
		gDirect->GetProjectionMatrix(projectionMatrix);

		mMesh->Render(deviceContext);

		TransformComponent* objectTransform = mOwnerGameObject->GetComponent<TransformComponent>();

		mBumpShader->Render(deviceContext, mMesh->GetIndexCount(), objectTransform->GetTransform(), viewMatrix, projectionMatrix, mTexture->GetTexture(), mBumpTexture->GetTexture(), mSpecularTexture->GetTexture(), gMainLight->GetPosition(), gMainLight->GetDiffuseColor(), gMainLight->GetAmbientColor(), gMainCamera->GetPosition(), gMainLight->GetSpecularColor(), gMainLight->GetSpecularPower(), mUpdateBone);
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
	Texture* mBumpTexture;
	Texture* mSpecularTexture;
};

DECLARE_COMPONENT(SkinnedMeshBumpRenderComponent);
