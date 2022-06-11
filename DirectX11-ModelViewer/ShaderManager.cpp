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

    mHdrShader = new HdrShader;
    result = mHdrShader->Initialize(device, hwnd);
    if (result == false)
    {
        return false;
    }

    mHorizontalBlurShader = new HorizontalBlurShaderClass;
    result = mHorizontalBlurShader->Initialize(device, hwnd);
    if (result == false)
    {
        return false;
    }

    mVerticalBlurShader = new VerticalBlurShaderClass;
    result = mVerticalBlurShader->Initialize(device, hwnd);
    if (result == false)
    {
        return false;
    }

    mBloomResultShader = new BloomResultShaderClass;
    result = mBloomResultShader->Initialize(device, hwnd);
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

HdrShader* ShaderManager::GetHdrShader() const
{
    return mHdrShader;
}

HorizontalBlurShaderClass* ShaderManager::GetHorizonBlurShader() const
{
    return mHorizontalBlurShader;
}

VerticalBlurShaderClass* ShaderManager::GetVerticalBlurShader() const
{
    return mVerticalBlurShader;
}

BloomResultShaderClass* ShaderManager::GetBloomResultShader() const
{
    return mBloomResultShader;
}

void ShaderManager::Shutdown()
{
    if (mLightMeshShader != nullptr)
    {
        mLightMeshShader->Shutdown();
        delete mLightMeshShader;
        mLightMeshShader = nullptr;
    }

    if (mHdrShader != nullptr)
    {
        mHdrShader->Shutdown();
        delete mHdrShader;
        mHdrShader = nullptr;
    }

    if (mHorizontalBlurShader != nullptr)
    {
        mHorizontalBlurShader->Shutdown();
        delete mHorizontalBlurShader;
        mHorizontalBlurShader = nullptr;
    }

    if (mVerticalBlurShader != nullptr)
    {
        mVerticalBlurShader->Shutdown();
        delete mVerticalBlurShader;
        mVerticalBlurShader = nullptr;
    }

    if (mBloomResultShader != nullptr)
    {
        mBloomResultShader->Shutdown();
        delete mBloomResultShader;
        mBloomResultShader = nullptr;
    }
}
