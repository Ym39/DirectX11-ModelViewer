#pragma once
#include "Singleton.h"
#include "LightMeshShader.h"

class ShaderManager : public Singletone<ShaderManager>
{
public:
	bool Initialize(ID3D11Device* device, HWND hwnd);

	LightMeshShader* GetLighatMeshShader() const;

private:
	LightMeshShader* mLightMeshShader;
};

