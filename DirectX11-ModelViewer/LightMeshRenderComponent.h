#pragma once
#include"Component.h"
#include"MeshRenderComponent.h"
#include"Camera.h"
#include"D3DClass.h"
#include"RenderTextureClass.h"
#include"LightManager.h"

extern Camera* gMainCamera;
extern D3DClass* gDirect;
extern RenderTextureClass* gDepthTexture;

class LightMeshRenderComponenet : public MeshRenderComponent
{
public:
	DEFINE_COMPONENT(LightMeshRenderComponenet, MeshRenderComponent, false);

public:
	LightMeshRenderComponenet()
	{
		mRendererType = eRendererType::LightMeshRenderer;
	}
	~LightMeshRenderComponenet() = default;

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

	MeshClass* GetMesh() { return mMesh; }

	eRendererType GetRenderType() const { return mRendererType; }

private:
	vector<vector<SpcularMaterial>> mSubObjectMats;
};