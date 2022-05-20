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
#include"BoundModel.h"
#include"SimpleColorShader.h"
#include <d3d11.h>

extern Camera* gMainCamera;
extern Light* gMainLight;
extern D3DClass* gDirect;
extern SimpleColorShader* gSimpleColorShader;

enum class eRendererType
{
	MeshRenderer,
	SkinnedMeshRenderer,
	SkinnedBumpRenderer,
	ReceiveShadowRenderer,
	LightMeshRenderer
};

class MeshRenderComponent : public BaseComponent
{
public:
	DEFINE_COMPONENT(MeshRenderComponent, BaseComponent, true);

public:
	MeshRenderComponent() :
		mMesh(nullptr),
		mShader(nullptr),
		mDepthShader(nullptr),
		mVisibleBoundBox(false)
	{
		mRendererType = eRendererType::MeshRenderer;
	}
	~MeshRenderComponent() = default;

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

		if(mVisibleBoundBox == true)
		   RenderBoundBox(deviceContext, objectTransform->GetTransform(), viewMatrix, projectionMatrix);
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

		mBoundModel = new BoundModel(gDirect->GetDevice(), mMesh->GetBounds());

		return true;
	}

	const auto& GetObjectMaterials() const { return mSubObjectMats; }
	void SetMaterial(int groupIndex,int submeshIndex, string textureKey)
	{
		mSubObjectMats[groupIndex][submeshIndex].SetTextureKey(textureKey);
	}
	MeshClass* GetMesh() { return mMesh; }

	eRendererType GetRenderType() const { return mRendererType; }

	void SetVisibleBoundsBox(bool visible) { mVisibleBoundBox = visible; }
	bool IsVisibleBoundsBox()const { return mVisibleBoundBox; }
protected:
	void RenderBoundBox(ID3D11DeviceContext* deviceContext,XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix)
	{
		mBoundModel->Render(deviceContext);
		gSimpleColorShader->Render(deviceContext, mBoundModel->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, XMFLOAT4(0.f, 1.f, 0.f, 1.f));
	}

protected:
	MeshClass* mMesh;
	eRendererType mRendererType;
	BoundModel* mBoundModel;

	bool mVisibleBoundBox;
private:
	SpecularShaderClass* mShader;
	DepthShaderClass* mDepthShader;

	vector<vector<SpcularMaterial>> mSubObjectMats;
};

DECLARE_COMPONENT(MeshRenderComponent);

