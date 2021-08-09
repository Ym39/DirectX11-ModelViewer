#pragma once
#include"MeshRenderComponent.h"
#include"TransformComponent.h"
#include "SkinnedMeshShader.h"
#include "SkinnedDepthShaderClass.h"
#include"Camera.h"
#include"Light.h"
#include"AnimatorComponent.h"
#include"Material.h"

extern Camera* gMainCamera;
extern Light* gMainLight;
extern D3DClass* gDirect;

class SkinnedMeshRenderComponent : public MeshRenderComponent
{
public:
	DEFINE_COMPONENT(SkinnedMeshRenderComponent, MeshRenderComponent, false);

public:
	SkinnedMeshRenderComponent():
		MeshRenderComponent(),
		mShader(nullptr),
		mDepthShader(nullptr)
	{
		mRendererType = eRendererType::SkinnedMeshRenderer;
	}
	~SkinnedMeshRenderComponent() = default;

	bool Initalize(MeshClass* mesh, SkinnedMeshShader* shader, SkinnedDepthShaderClass* depthShader)
	{
		if (mesh == nullptr || shader == nullptr || depthShader == nullptr)
			return false;

		if (mesh->IsInitalized() == false)
			return false;

		mMesh = mesh;
		mShader = shader;
		mDepthShader = depthShader;

		mUpdateBone.resize(mMesh->GetBones().size());

		for (auto& b : mUpdateBone)
		{
			b = XMMatrixTranspose(XMMatrixIdentity());
		}

		for (int i = 0; i < mMesh->GetSubMeshCount(); i++)
		{
			mSubObjectMats[mMesh->GetSubMeshName(i)] = SpcularMaterial();
		}

		return true;
	}

	virtual void RenderDepth(ID3D11DeviceContext* deviceContext) override
	{
		if (mMesh == nullptr )
			return;

		XMMATRIX lightViewMatrix, lightProjectionMatrix;
		gMainLight->GetViewMatrix(lightViewMatrix);
		gMainLight->GetProjectionMatrix(lightProjectionMatrix);

	
		TransformComponent* objectTransform = mOwnerGameObject->GetComponent<TransformComponent>();

		for (int i = 0; i < mMesh->GetSubMeshCount(); i++)
		{
			mMesh->Render(deviceContext, i);

			mDepthShader->Render(deviceContext, mMesh->GetSubMeshIndexCount(i), objectTransform->GetTransform(), lightViewMatrix, lightProjectionMatrix, mUpdateBone);
		}
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

			SpcularMaterial& mat = mSubObjectMats[mMesh->GetSubMeshName(i)];

			mShader->Render(deviceContext, mMesh->GetSubMeshIndexCount(i), objectTransform->GetTransform(), viewMatrix, projectionMatrix, AssetClass::mTextureMap[mat.GetTextureKey()]->GetTexture(), gMainLight->GetPosition(), gMainLight->GetDiffuseColor(), gMainLight->GetAmbientColor(), gMainCamera->GetPosition(), gMainLight->GetSpecularColor(), gMainLight->GetSpecularPower(), mUpdateBone);
		}
	}
	const auto& GetObjectMaterials() const { return mSubObjectMats; }
	void SetMaterial(string key, string textureKey)
	{
		mSubObjectMats[key].SetTextureKey(textureKey);
	}
public:
	virtual void Start() override {};
	virtual void Update(float deltaTime) override {};
	virtual void LateUpdate(float deltaTime) override
	{
		static float lateTime = 0.0f;

		AnimatorComponent* animator = mOwnerGameObject->GetComponent<AnimatorComponent>();

		if (animator == nullptr)
			return;

		if (animator->IsPlay() == false)
			return;

		lateTime += deltaTime;

		if (lateTime >= 1000.0f / 30.0f)
		{
			for (int i = 0; i < mUpdateBone.size(); i++)
			{
				XMMATRIX boneOffset = XMLoadFloat4x4(&(mMesh->GetBones()[i].globalBindposeInverse));
				XMMATRIX toParent = XMLoadFloat4x4(&animator->GetCurrentKeyAnimation(i).globalTransfrom);
				XMMATRIX finalTransform = XMMatrixMultiply(boneOffset, toParent) * XMMatrixTranslation(0.f, 200.f, 0.f);
				finalTransform *= XMMatrixScaling(1.0f, 1.0f, 1.0f);
				mUpdateBone[i] = XMMatrixTranspose(finalTransform);
			}

			lateTime = 0.0f;
		}
	}

	virtual void Destroy() override {};

protected:
	SkinnedMeshShader* mShader;
	SkinnedDepthShaderClass* mDepthShader;
	vector<XMMATRIX> mUpdateBone;
private:
	unordered_map<string, SpcularMaterial> mSubObjectMats;
};

DECLARE_COMPONENT(SkinnedMeshRenderComponent);

