#pragma once
#include "Mesh.h"
#include "Texture.h"
#include "Transform.h"
class GameObject
{
public:
    GameObject();
    ~GameObject();

    void SetMesh(Mesh* mesh);
    void SetTexture(Texture* texture);
    Transform& Transfrom();

    void Shutdown();
private:
    Mesh* mMesh;
    Texture* mTexture;
    Transform mTransform;
};

