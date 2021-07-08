#pragma once
#include"Component.h"
#include"D3DClass.h"
#include"MeshClass.h"
#include"SpecularShaderClass.h"
#include"Texture.h"
#include <d3d11.h>

extern Camera* gMainCamera;
extern Light* gMainLight;
extern D3DClass* gDirect;

class MeshRenderComponent : public BaseComponent
{
public:
	DEFINE_COMPONENT(MeshRenderComponent, BaseComponent, true);

public:
	MeshRenderComponent() :
		mMesh(nullptr),
		mShader(nullptr),
		mTexture(nullptr)
	{}
	~MeshRenderComponent() = default;

public:
	virtual void Start() override {};
	virtual void Update() override {};
	virtual void LateUpdate() override {};
	virtual void Destroy() override {};

	virtual void Render(ID3D11DeviceContext* deviceContext)
	{
		XMMATRIX viewMatrix, projectionMatrix;
		gMainCamera->GetViewMatrix(viewMatrix);
		gDirect->GetOrthoMatrix(projectionMatrix);

		mMesh->Render(deviceContext);

		TransformComponent* objectTransform = mOwnerGameObject->GetComponent<TransformComponent>();

		mShader->Render(deviceContext, mMesh->GetIndexCount(), objectTransform->GetTransform(), viewMatrix, projectionMatrix, mTexture->GetTexture(), gMainLight->GetPosition(), gMainLight->GetDiffuseColor(), gMainLight->GetAmbientColor(), gMainCamera->GetPosition(), gMainLight->GetSpecularColor(), gMainLight->GetSpecularPower());
	}

public:
	bool Initalize(MeshClass* mesh, SpecularShaderClass* shader, Texture* texture)
	{
		if (mesh == nullptr || shader == nullptr || texture == nullptr)
			return false;

		if (mesh->IsInitalized() == false)
			return false;

		mMesh = mesh;
		mShader = shader;
		mTexture = texture;

		return true;
	}

protected:
	MeshClass* mMesh;

private:
	SpecularShaderClass* mShader;
	Texture* mTexture;
};

DECLARE_COMPONENT(MeshRenderComponent);

