#include "LightMeshShader.h"
#include "AssetClass.h"

LightMeshShader::LightMeshShader() :
	mVertexShader(nullptr),
	mPixelShader(nullptr),
	mLayout(nullptr),
	mMaterialBuffer(nullptr),
	mLightBuffer(nullptr),
	mMatrixBuffer(nullptr),
	mSrv(nullptr),
	mSampleStateWarp(nullptr),
	mSampleStateClamp(nullptr)
{
}

LightMeshShader::~LightMeshShader()
{
}

bool LightMeshShader::Initialize(ID3D11Device* device, HWND hwnd)
{
	return InitializeShader(device, hwnd, L"LightMeshVertexShader.hlsl", L"LightMeshPixelShader.hlsl");
}

void LightMeshShader::Shutdown()
{
	ShutdownShader();
}

bool LightMeshShader::Render(ID3D11DeviceContext* deviceContext, int indexCount, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix, WideMaterial* material)
{
	bool result;

	result = SetShaderParameters(deviceContext, worldMatrix, viewMatrix, projectionMatrix, material);
	if (result == false)
	{
		return false;
	}

	RenderShader(deviceContext, indexCount);

	return true;
}

bool LightMeshShader::InitializeShader(ID3D11Device* device, HWND hwnd, WCHAR* vsFilename, WCHAR* psFilename)
{
	HRESULT result;
	ID3D10Blob* errorMessage;
	ID3D10Blob* vertexShaderBuffer;
	ID3D10Blob* pixelShaderBuffer;
	D3D11_INPUT_ELEMENT_DESC polygonLayout[5];
	unsigned int numElements;
	D3D11_BUFFER_DESC matrixBufferDesc;

	result = D3DCompileFromFile(vsFilename, NULL, NULL, "VS", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &vertexShaderBuffer, &errorMessage);

	if (FAILED(result))
	{
		if (errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, hwnd, vsFilename);
		}
		else
		{
			MessageBox(hwnd, vsFilename, L"Missing Shader File", MB_OK);
		}

		return false;
	}

	result = D3DCompileFromFile(psFilename, NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, "PS", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &pixelShaderBuffer, &errorMessage);


	if (FAILED(result))
	{
		if (errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, hwnd, psFilename);
		}
		else
		{
			MessageBox(hwnd, psFilename, L"Missing Shader File", MB_OK);
		}

		return false;
	}

	result = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &mVertexShader);
	if (FAILED(result))
	{

		return false;
	}

	result = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &mPixelShader);
	if (FAILED(result))
	{

		return false;
	}

	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "TEXCOORD";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	polygonLayout[2].SemanticName = "NORMAL";
	polygonLayout[2].SemanticIndex = 0;
	polygonLayout[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[2].InputSlot = 0;
	polygonLayout[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[2].InstanceDataStepRate = 0;

	polygonLayout[3].SemanticName = "BINORMAL";
	polygonLayout[3].SemanticIndex = 0;
	polygonLayout[3].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[3].InputSlot = 0;
	polygonLayout[3].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[3].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[3].InstanceDataStepRate = 0;

	polygonLayout[4].SemanticName = "TANGENT";
	polygonLayout[4].SemanticIndex = 0;
	polygonLayout[4].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[4].InputSlot = 0;
	polygonLayout[4].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[4].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[4].InstanceDataStepRate = 0;

	numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	result = device->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(),
		vertexShaderBuffer->GetBufferSize(), &mLayout);
	if (FAILED(result))
	{
		return false;
	}

	vertexShaderBuffer->Release();
	vertexShaderBuffer = nullptr;

	pixelShaderBuffer->Release();
	pixelShaderBuffer = nullptr;

	D3D11_SAMPLER_DESC samplerDesc;
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	result = device->CreateSamplerState(&samplerDesc, &mSampleStateWarp);
	if (FAILED(result))
	{
		return false;
	}

	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;

	result = device->CreateSamplerState(&samplerDesc, &mSampleStateClamp);
	if (FAILED(result))
	{
		return false;
	}

	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(PerObejct);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	result = device->CreateBuffer(&matrixBufferDesc, NULL, &mMatrixBuffer);
	if (FAILED(result))
	{
		return false;
	}

	D3D11_BUFFER_DESC materialBufferDesc;
	materialBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	materialBufferDesc.ByteWidth = sizeof(MaterialBuffer);
	materialBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	materialBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	materialBufferDesc.MiscFlags = 0;
	materialBufferDesc.StructureByteStride = 0;
	result = device->CreateBuffer(&materialBufferDesc, NULL, &mMaterialBuffer);
	if (FAILED(result))
	{
		return false;
	}

	D3D11_BUFFER_DESC lightBufferDesc;
	lightBufferDesc.ByteWidth = sizeof(LightBuffer) * NUM_OF_LIGHTS;
	lightBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	lightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	lightBufferDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	lightBufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	lightBufferDesc.StructureByteStride = sizeof(LightBuffer);

	D3D11_SUBRESOURCE_DATA subResourceData;
	subResourceData.pSysMem = 0;
	subResourceData.SysMemPitch = 0;
	subResourceData.SysMemSlicePitch = 0;

	result = device->CreateBuffer(&lightBufferDesc, NULL, &mLightBuffer);
	if (FAILED(result))
	{
		return false;
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	srvDesc.Buffer.FirstElement = 0;
	srvDesc.Buffer.NumElements = 1;

	result = device->CreateShaderResourceView(mLightBuffer, &srvDesc, &mSrv);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

void LightMeshShader::ShutdownShader()
{
	if (mVertexShader)
	{
		mVertexShader->Release();
		mVertexShader = nullptr;
	}

	if (mPixelShader)
	{
		mPixelShader->Release();
		mPixelShader = nullptr;
	}

	if (mLayout)
	{	
		mLayout->Release();
		mLayout = nullptr;
	}

	if (mMatrixBuffer)
	{
		mMatrixBuffer->Release();
		mMatrixBuffer = nullptr;
	}

	if (mMaterialBuffer)
	{
		mMaterialBuffer->Release();
		mMaterialBuffer = nullptr;
	}

	if (mLightBuffer)
	{
		mLightBuffer->Release();
		mLightBuffer = nullptr;
	}

	if (mSrv)
	{
		mSrv->Release();
		mSrv = nullptr;
	}

	if (mSampleStateWarp)
	{
		mSampleStateWarp->Release();
		mSampleStateWarp = nullptr;
	}

	if (mSampleStateClamp)
	{
		mSampleStateClamp->Release();
		mSampleStateClamp = nullptr;
	}
}

void LightMeshShader::OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, WCHAR* shaderFilename)
{
	char* compileErrors;
	unsigned long long bufferSize, i;
	ofstream fout;

	compileErrors = (char*)(errorMessage->GetBufferPointer());

	// Get the length of the message.
	bufferSize = errorMessage->GetBufferSize();

	// Open a file to write the error message to.
	fout.open("shader-error.txt");

	// Write out the error message.
	for (i = 0; i < bufferSize; i++)
	{
		fout << compileErrors[i];
	}

	// Close the file.
	fout.close();

	// Release the error message.
	errorMessage->Release();
	errorMessage = 0;

	// Pop a message up on the screen to notify the user to check the text file for compile errors.
	MessageBox(hwnd, L"Error compiling shader.  Check shader-error.txt for message.", shaderFilename, MB_OK);

	return;
}

bool LightMeshShader::SetShaderParameters(ID3D11DeviceContext* deviceContext, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix, WideMaterial* material)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	unsigned int bufferNumber;

	//worldMatrix = XMMatrixTranspose(worldMatrix);
	//viewMatrix = XMMatrixTranspose(viewMatrix);
	//projectionMatrix = XMMatrixTranspose(projectionMatrix);
	
	result = deviceContext->Map(mMatrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	PerObejct* dataPtr = (PerObejct*)mappedResource.pData;

	dataPtr->modelView = XMMatrixMultiply(worldMatrix, viewMatrix);
	dataPtr->modelViewProjection = XMMatrixMultiply(dataPtr->modelView, projectionMatrix);

	deviceContext->Unmap(mMatrixBuffer, 0);
	bufferNumber = 0;

	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &mMatrixBuffer);

	//텍스쳐 셋팅
	int textureNum = 0;

	deviceContext->PSSetShaderResources(0, 1, AssetClass::mTextureMap[material->GetAmbientKey()]->GetTexturePP());
	deviceContext->PSSetShaderResources(1, 1, AssetClass::mTextureMap[material->GetEmissiveKey()]->GetTexturePP());
	deviceContext->PSSetShaderResources(2, 1, AssetClass::mTextureMap[material->GetDiffuseKey()]->GetTexturePP());
	deviceContext->PSSetShaderResources(3, 1, AssetClass::mTextureMap[material->GetSpecularKey()]->GetTexturePP());
	deviceContext->PSSetShaderResources(4, 1, AssetClass::mTextureMap[material->GetSpecularPowerKey()]->GetTexturePP());
	deviceContext->PSSetShaderResources(5, 1, AssetClass::mTextureMap[material->GetNormalKey()]->GetTexturePP());
	deviceContext->PSSetShaderResources(6, 1, AssetClass::mTextureMap[material->GetBumpKey()]->GetTexturePP());
	deviceContext->PSSetShaderResources(7, 1, AssetClass::mTextureMap[material->GetOpacityKey()]->GetTexturePP());

	result = deviceContext->Map(mMaterialBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	MaterialBuffer* dataPtr2 = (MaterialBuffer*)mappedResource.pData;

	dataPtr2->mat = material->shaderUploadMaterial;

	deviceContext->Unmap(mMaterialBuffer, 0);

	++bufferNumber;

	deviceContext->PSSetConstantBuffers(bufferNumber, 1, &mMaterialBuffer);

	result = deviceContext->Map(mLightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	LightBuffer *dataPtr3 = (LightBuffer*)mappedResource.pData;
	LightBuffer dummyLight;

	size_t sizeInBytes = NUM_OF_LIGHTS * sizeof(LightBuffer);
	memcpy_s(mappedResource.pData, sizeInBytes, LightManager::Instance().GetAllLight(), sizeInBytes);

	deviceContext->Unmap(mLightBuffer, 0);
	deviceContext->PSSetShaderResources(8, 1, &mSrv);

	return true;
}

void LightMeshShader::RenderShader(ID3D11DeviceContext* deviceContext, int indexCount)
{
	deviceContext->IASetInputLayout(mLayout);

	deviceContext->VSSetShader(mVertexShader, NULL, 0);
	deviceContext->PSSetShader(mPixelShader, NULL, 0);

	deviceContext->PSSetSamplers(0, 1, &mSampleStateClamp);
	deviceContext->PSSetSamplers(1, 1, &mSampleStateWarp);

	deviceContext->DrawIndexed(indexCount, 0, 0);

	return;
}
