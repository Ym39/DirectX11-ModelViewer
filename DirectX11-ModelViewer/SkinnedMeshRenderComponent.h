#pragma once
#include"MeshRenderComponent.h"
#include"TransformComponent.h"
#include "SkinnedMeshShader.h"
#include "SkinnedDepthShaderClass.h"
#include"Camera.h"
#include"Light.h"
#include"AnimatorComponent.h"

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
		mDepthShader(nullptr),
		mTexture(nullptr)
	{}
	~SkinnedMeshRenderComponent() = default;

	bool Initalize(MeshClass* mesh, SkinnedMeshShader* shader, SkinnedDepthShaderClass* depthShader, Texture* texture)
	{
		if (mesh == nullptr || shader == nullptr || texture == nullptr || depthShader == nullptr)
			return false;

		if (mesh->IsInitalized() == false)
			return false;

		mMesh = mesh;
		mShader = shader;
		mTexture = texture;
		mDepthShader = depthShader;

		mUpdateBone.resize(mMesh->GetBones().size());

		for (auto& b : mUpdateBone)
		{
			b = XMMatrixTranspose(XMMatrixIdentity());
		}

		return true;
	}

	virtual void RenderDepth(ID3D11DeviceContext* deviceContext) override
	{
		if (mMesh == nullptr || mTexture == nullptr)
			return;

		XMMATRIX lightViewMatrix, lightProjectionMatrix;
		gMainLight->GetViewMatrix(lightViewMatrix);
		gMainLight->GetProjectionMatrix(lightProjectionMatrix);

		mMesh->Render(deviceContext);

		TransformComponent* objectTransform = mOwnerGameObject->GetComponent<TransformComponent>();

		mDepthShader->Render(deviceContext, mMesh->GetIndexCount(), objectTransform->GetTransform(), lightViewMatrix, lightProjectionMatrix, mUpdateBone);
	}

	virtual void Render(ID3D11DeviceContext* deviceContext) override
	{
		if (mMesh == nullptr || mTexture == nullptr)
			return;

		XMMATRIX viewMatrix, projectionMatrix;
		gMainCamera->GetViewMatrix(viewMatrix);
		gDirect->GetProjectionMatrix(projectionMatrix);

		mMesh->Render(deviceContext);

		TransformComponent* objectTransform = mOwnerGameObject->GetComponent<TransformComponent>();

		mShader->Render(deviceContext, mMesh->GetIndexCount(), objectTransform->GetTransform(), viewMatrix, projectionMatrix, mTexture->GetTexture(), gMainLight->GetPosition(), gMainLight->GetDiffuseColor(), gMainLight->GetAmbientColor(), gMainCamera->GetPosition(), gMainLight->GetSpecularColor(), gMainLight->GetSpecularPower(), mUpdateBone);
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
	Texture* mTexture;

	vector<XMMATRIX> mUpdateBone;
};

DECLARE_COMPONENT(SkinnedMeshRenderComponent);

