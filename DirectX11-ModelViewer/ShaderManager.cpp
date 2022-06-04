#include "ShaderManager.h"

bool ShaderManager::Initialize(ID3D11Device* device, HWND hwnd)
{
    bool result = false;

    mLightMeshShader = new LightMeshShader;
    result = mLightMeshShader->Initialize(device, hwnd);
    if (result == false)
    {
        return false;
    }

    return true;
}

LightMeshShader* ShaderManager::GetLighatMeshShader() const
{
    return mLightMeshShader;
}

void ShaderManager::Shutdown()
{
    if (mLightMeshShader != nullptr)
    {
        mLightMeshShader->Shutdown();
        delete mLightMeshShader;
        mLightMeshShader = nullptr;
    }
}
