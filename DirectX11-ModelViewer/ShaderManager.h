#pragma once
#include "Singleton.h"
#include "LightMeshShader.h"
#include "HdrShader.h"
#include "HorizontalBlurShaderClass.h"
#include "VerticalBlurShaderClass.h"
#include "BloomResultShaderClass.h"

class ShaderManager : public Singletone<ShaderManager>
{
public:
	bool Initialize(ID3D11Device* device, HWND hwnd);

	LightMeshShader* GetLighatMeshShader() const;
	HdrShader* GetHdrShader() const;
	HorizontalBlurShaderClass* GetHorizonBlurShader() const;
	VerticalBlurShaderClass* GetVerticalBlurShader() const;
	BloomResultShaderClass* GetBloomResultShader() const;

	void Shutdown();

private:
	LightMeshShader* mLightMeshShader;
	HdrShader* mHdrShader;
	HorizontalBlurShaderClass* mHorizontalBlurShader;
	VerticalBlurShaderClass* mVerticalBlurShader;
	BloomResultShaderClass* mBloomResultShader;
};

