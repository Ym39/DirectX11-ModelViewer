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

		for (int i = 0; i < mMesh->GetSubMeshGroupCount(); i++)
		{
			for (int j = 0; j < mMesh->GetSubMeshCount(i); j++)
			{
				mMesh->Render(deviceContext, i , j);
				mDepthShader->Render(deviceContext, mMesh->GetSubMeshIndexCount(i,j), objectTransform->GetTransform(), lightViewMatrix, lightProjectionMatrix);
			}
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

		for (int i = 0; i < mMesh->GetSubMeshGroupCount(); i++)
		{
			for (int j = 0; j < mMesh->GetSubMeshCount(i); j++)
			{
				mMesh->Render(deviceContext, i,j);

				SpcularMaterial& mat = mSubObjectMats[i][j];

				mShader->Render(deviceContext, mMesh->GetSubMeshIndexCount(i,j), objectTransform->GetTransform(), viewMatrix, projectionMatrix, AssetClass::mTextureMap[mat.GetTextureKey()]->GetTexture(), gMainLight->GetPosition(), gMainLight->GetDiffuseColor(), gMainLight->GetAmbientColor(), gMainCamera->GetPosition(), gMainLight->GetSpecularColor(), gMainLight->GetSpecularPower());
			}
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

		mSubObjectMats.resize(mMesh->GetSubMeshGroupCount());
		for (int i = 0; i < mSubObjectMats.size(); i++)
		{
			mSubObjectMats[i].resize(mMesh->GetSubMeshCount(i));
		}

		return true;
	}

	const auto& GetObjectMaterials() const { return mSubObjectMats; }
	void SetMaterial(int groupIndex,int submeshIndex, string textureKey)
	{
		mSubObjectMats[groupIndex][submeshIndex].SetTextureKey(textureKey);
	}

	eRendererType GetRenderType() const { return mRendererType; }

protected:
	MeshClass* mMesh;
	eRendererType mRendererType;
private:
	SpecularShaderClass* mShader;
	DepthShaderClass* mDepthShader;

	vector<vector<SpcularMaterial>> mSubObjectMats;
};

DECLARE_COMPONENT(MeshRenderComponent);

