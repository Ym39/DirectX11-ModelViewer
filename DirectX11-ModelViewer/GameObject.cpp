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

