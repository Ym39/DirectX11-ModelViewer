#pragma once
#include"Component.h"
#include"D3DClass.h"
#include"MeshClass.h"
#include"SpecularShaderClass.h"
#include"DepthShaderClass.h"
#include"TransformComponent.h"
#include"Texture.h"
#include"Camera.h"
#include"Light.h"
#include"Material.h"
#include <d3d11.h>

extern Camera* gMainCamera;
extern Light* gMainLight;
extern D3DClass* gDirect;

enum class eRendererType
{
	MeshRenderer,
	SkinnedMeshRenderer,
	SkinnedBumpRenderer
};

class MeshRenderComponent : public BaseComponent
{
public:
	DEFINE_COMPONENT(MeshRenderComponent, BaseComponent, true);

public:
	MeshRenderComponent() :
		mMesh(nullptr),
		mShader(nullptr),
		mDepthShader(nullptr)
	{
		mRendererType = eRendererType::MeshRenderer;
	}
	~MeshRenderComponent() = default;

public:
	virtual void Start() override {};
	virtual void Update(float deltaTime) override {};
	virtual void LateUpdate(float deltaTime) override {};
	virtual void Destroy() override {};

	virtual void RenderDepth(ID3D11DeviceContext* deviceContext)
	{
		if (mMesh == nullptr)
			return;

		XMMATRIX lightViewMatrix, lightProjectionMatrix;
		gMainLight->GetViewMatrix(lightViewMatrix);
		gMainLight->GetProjectionMatrix(lightProjectionMatrix);

		TransformComponent* objectTransform = mOwnerGameObject->GetComponent<TransformComponent>();

		for (int i = 0; i < mMesh->GetSubMeshCount(); i++)
		{
			mMesh->Render(deviceContext, i);

			mDepthShader->Render(deviceContext, mMesh->GetSubMeshIndexCount(i), objectTransform->GetTransform(), lightViewMatrix, lightProjectionMatrix);
		}
	}

	virtual void Render(ID3D11DeviceContext* deviceContext)
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

			mShader->Render(deviceContext, mMesh->GetSubMeshIndexCount(i), objectTransform->GetTransform(), viewMatrix, projectionMatrix, AssetClass::mTextureMap[mat.GetTextureKey()]->GetTexture(), gMainLight->GetPosition(), gMainLight->GetDiffuseColor(), gMainLight->GetAmbientColor(), gMainCamera->GetPosition(), gMainLight->GetSpecularColor(), gMainLight->GetSpecularPower());
		}
	}

public:
	bool Initalize(MeshClass* mesh, SpecularShaderClass* shader, DepthShaderClass* depthShader)
	{
		if (mesh == nullptr || shader == nullptr || depthShader == nullptr)
			return false;

		if (mesh->IsInitalized() == false)
			return false;

		mMesh = mesh;
		mShader = shader;
		mDepthShader = depthShader;

		for (int i = 0; i < mMesh->GetSubMeshCount(); i++)
		{
			mSubObjectMats[mMesh->GetSubMeshName(i)] = SpcularMaterial();
		}

		return true;
	}

	const auto& GetObjectMaterials() const { return mSubObjectMats; }
	void SetMaterial(string key, string textureKey)
	{
		mSubObjectMats[key].SetTextureKey(textureKey);
	}

	eRendererType GetRenderType() const { return mRendererType; }

protected:
	MeshClass* mMesh;
	eRendererType mRendererType;
private:
	SpecularShaderClass* mShader;
	DepthShaderClass* mDepthShader;

	unordered_map<string, SpcularMaterial> mSubObjectMats;
};

DECLARE_COMPONENT(MeshRenderComponent);

