#include "GameObject.h"

GameObject::GameObject():
mMesh(nullptr),
mTexture(nullptr),
mIsAnimationPlay(false),
mIsHasAnimation(false)
{
    
}

GameObject::~GameObject()
{

}

bool GameObject::Initailize(ID3D11Device* device,HWND hwnd)
{
    shader = new SkinnedMeshShader;
    bool result = shader->Initialize(device, hwnd);
    if (result== false)
    {
        return false;
    }

    return true;
}

void GameObject::SetMesh(Mesh* mesh)
{
    mMesh = mesh;
    mIsHasAnimation = mMesh->IsSkinned();
}

void GameObject::SetTexture(Texture* texture)
{
    mTexture = texture;
}

Transform& GameObject::Transfrom()
{
    return mTransform;
}

void GameObject::Update(float deltaTime)
{
    if (mIsHasAnimation == false)
        return;

    if (mIsAnimationPlay == true)
    {
        mMesh->UpdateAnimation(deltaTime);
    }
}

void GameObject::Render(ID3D11DeviceContext* deviceContext)
{
    mMesh->Render(deviceContext);
}

bool GameObject::Render(ID3D11DeviceContext* deviceContext, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix, ID3D11ShaderResourceView* texture, XMFLOAT3 lightPosition, XMFLOAT4 diffuseColor, XMFLOAT4 ambientColor, XMFLOAT3 cameraPosition, XMFLOAT4 specularColor, float specularPower)
{

    mMesh->Render(deviceContext);

    bool result = shader->Render(deviceContext, mMesh->GetIndexCount(), mTransform.GetTransform(), viewMatrix, projectionMatrix, mTexture->GetTexture(), lightPosition, diffuseColor, ambientColor, cameraPosition,specularColor, specularPower,mMesh->GetBoneTransform());

    if (result == false)
        return false;

    return true;
}

void GameObject::Shutdown()
{
    if (mMesh)
    {
        mMesh->Shutdown();
        delete mMesh;
        mMesh = nullptr;
    }

    if (mTexture)
    {
        mTexture->Shutdown();
        delete mTexture;
        mTexture = nullptr;
    }
}

ID3D11ShaderResourceView* GameObject::GetTexture() const
{
    return mTexture->GetTexture();
}

int GameObject::GetIndexCount() const
{
    return mMesh->GetIndexCount();
}

