#include "GameObject.h"

GameObject::GameObject():
mMesh(nullptr),
mTexture(nullptr)
{
    
}

GameObject::~GameObject()
{
}

void GameObject::SetMesh(Mesh* mesh)
{
    mMesh = mesh;
}

void GameObject::SetTexture(Texture* texture)
{
    mTexture = texture;
}

Transform& GameObject::Transfrom()
{
    return mTransform;
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
