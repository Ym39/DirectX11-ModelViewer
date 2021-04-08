#pragma once
#include "Mesh.h"
#include "Texture.h"
#include "Transform.h"
#include "SkinnedMeshShader.h"
class GameObject
{
public:
    GameObject();
    ~GameObject();

    void SetMesh(Mesh* mesh);
    void SetTexture(Texture* texture);
    Transform& Transfrom();

    void Update(float deltaTime);
    void Render(ID3D11DeviceContext* deviceContext);

    void Shutdown();

    void PlayAnimation(bool result){mIsAnimationPlay = result; }
public:
    ID3D11ShaderResourceView* GetTexture() const;
    int GetIndexCount() const;
    const vector<XMMATRIX>& GetBoneTransform() { return mMesh->GetBoneTransform(); }
    const Skeleton& GetSkeleton() const { return mMesh->GetSkeleton(); }
    bool IsHasAnimation() const { return mIsHasAnimation; }

private:
    Mesh* mMesh;
    Texture* mTexture;
    SkinnedMeshShader* shader;

    Transform mTransform;
    
    bool mIsAnimationPlay;
    bool mIsHasAnimation;
};

