#pragma once
#include"Component.h"
#include"MeshRenderComponent.h"
#include"D3DClass.h"
#include"MeshClass.h"
#include"TransformComponent.h"
#include"Texture.h"
#include"Camera.h"
#include"Light.h"
#include"Material.h"
#include"BoundModel.h"
#include"ShadowShaderClass.h"
#include"AssetClass.h"
#include"RenderTextureClass.h"
#include <d3d11.h>

extern Camera* gMainCamera;
extern Light* gMainLight;
extern D3DClass* gDirect;
extern RenderTextureClass* gDepthTexture;

class ReceiveShadowRenderComponent : public MeshRenderComponent
{
public:
	DEFINE_COMPONENT(ReceiveShadowRenderComponent, MeshRenderComponent, false);

public:
	ReceiveShadowRenderComponent() :
		MeshRenderComponent(),
		mShader(nullptr),
		mDepthShader(nullptr)
	{
		mRendererType = eRendererType::ReceiveShadowRenderer;
	}
	~ReceiveShadowRenderComponent() = default;

public:
	virtual void Start() override {};
	virtual void Update(float deltaTime) override {};
	virtual void LateUpdate(float deltaTime) override {};
	virtual void Destroy() override
	{
		if (mBoundModel)
		{
			delete mBoundModel;
			mBoundModel = nullptr;
		}
	};

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
				mMesh->Render(deviceContext, i, j);
				mDepthShader->Render(deviceContext, mMesh->GetSubMeshIndexCount(i, j), objectTransform->GetTransform(), lightViewMatrix, lightProjectionMatrix);
			}
		}
	}

	virtual void Render(ID3D11DeviceContext* deviceContext)
	{
		if (mMesh == nullptr)
			return;

		XMMATRIX viewMatrix, projectionMatrix, lightViewMatrix, lightProjectionMatrix;
		gMainCamera->GetViewMatrix(viewMatrix);
		gDirect->GetProjectionMatrix(projectionMatrix);
		gMainLight->GetViewMatrix(lightViewMatrix);
		gMainLight->GetProjectionMatrix(lightProjectionMatrix);

		TransformComponent* objectTransform = mOwnerGameObject->GetComponent<TransformComponent>();

		for (int i = 0; i < mMesh->GetSubMeshGroupCount(); i++)
		{
			for (int j = 0; j < mMesh->GetSubMeshCount(i); j++)
			{
				mMesh->Render(deviceContext, i, j);

				SpcularMaterial& mat = mSubObjectMats[i][j];

				mShader->Render(deviceContext, mMesh->GetSubMeshIndexCount(i, j), objectTransform->GetTransform(), viewMatrix, projectionMatrix, lightViewMatrix, lightProjectionMatrix, AssetClass::mTextureMap[mat.GetTextureKey()]->GetTexture(), gDepthTexture->GetShaderResourceView(), gMainLight->GetPosition(), gMainLight->GetAmbientColor(), gMainLight->GetDiffuseColor(), gMainCamera->GetPosition());
			}
		}

		if (mVisibleBoundBox == true)
			RenderBoundBox(deviceContext, objectTransform->GetTransform(), viewMatrix, projectionMatrix);
	}

public:
	bool Initalize(MeshClass* mesh, ShadowShaderClass* shader, DepthShaderClass* depthShader)
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

		mBoundModel = new BoundModel(gDirect->GetDevice(), mMesh->GetBounds());

		return true;
	}

	const auto& GetObjectMaterials() const { return mSubObjectMats; }
	void SetMaterial(int groupIndex, int submeshIndex, string textureKey)
	{
		mSubObjectMats[groupIndex][submeshIndex].SetTextureKey(textureKey);
	}
	MeshClass* GetMesh() { return mMesh; }

	eRendererType GetRenderType() const { return mRendererType; }

	void SetVisibleBoundsBox(bool visible) { mVisibleBoundBox = visible; }
	bool IsVisibleBoundsBox()const { return mVisibleBoundBox; }
protected:
	void RenderBoundBox(ID3D11DeviceContext* deviceContext, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix)
	{
		mBoundModel->Render(deviceContext);
		gSimpleColorShader->Render(deviceContext, mBoundModel->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, XMFLOAT4(0.f, 1.f, 0.f, 1.f));
	}

protected:

private:
	ShadowShaderClass* mShader;
	DepthShaderClass* mDepthShader;

	vector<vector<SpcularMaterial>> mSubObjectMats;
};

DECLARE_COMPONENT(ReceiveShadowRenderComponent);