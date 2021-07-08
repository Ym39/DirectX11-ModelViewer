#include "MeshRenderComponent.h"
#include "TransformComponent.h"
#include "Camera.h"
#include "Light.h"

extern Camera* gMainCamera;
extern Light* gMainLight;
extern D3DClass* gDirect;

MeshRenderComponent::MeshRenderComponent():
	mMesh(nullptr),
	mShader(nullptr),
	mTexture(nullptr)
{
}

void MeshRenderComponent::Render(ID3D11DeviceContext* deviceContext)
{
	XMMATRIX viewMatrix, projectionMatrix;
	gMainCamera->GetViewMatrix(viewMatrix);
	gDirect->GetOrthoMatrix(projectionMatrix);

	mMesh->Render(deviceContext);

	TransformComponent* objectTransform = mOwnerGameObject->GetComponent<TransformComponent>();

	mShader->Render(deviceContext, mMesh->GetIndexCount(), objectTransform->GetTransform(), viewMatrix, projectionMatrix, mTexture->GetTexture(), gMainLight->GetPosition(), gMainLight->GetDiffuseColor(), gMainLight->GetAmbientColor(), gMainCamera->GetPosition(), gMainLight->GetSpecularColor(), gMainLight->GetSpecularPower());
}

bool MeshRenderComponent::Initalize(MeshClass* mesh, SpecularShaderClass* shader, Texture* texture)
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
