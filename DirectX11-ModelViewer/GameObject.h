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

    bool Initailize(ID3D11Device* device,HWND hwnd);

    void SetMesh(Mesh* mesh);
    void SetTexture(Texture* texture);
    Transform& Transfrom();

    void Update(float deltaTime);
    void Render(ID3D11DeviceContext* deviceContext);
    bool Render(ID3D11DeviceContext* deviceContext, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix, ID3D11ShaderResourceView* texture, XMFLOAT3 lightPosition, XMFLOAT4 diffuseColor, XMFLOAT4 ambientColor, XMFLOAT3 cameraPosition, XMFLOAT4 specularColor, float specularPower);

    void Shutdown();

public:
    ID3D11ShaderResourceView* GetTexture() const;
    int GetIndexCount() const;
    const vector<XMMATRIX>& GetBoneTransform() { return mMesh->GetBoneTransform(); }

private:
    Mesh* mMesh;
    Texture* mTexture;
    SkinnedMeshShader* shader;

    Transform mTransform;
    
    bool mIsAnimationPlay;
    bool mIsHasAnimation;
};

