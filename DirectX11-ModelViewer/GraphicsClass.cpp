#include "GraphicsClass.h"
#include"SystemClass.h"

extern SystemClass* ApplicationHandle;

GraphicsClass::GraphicsClass():
mDirect(nullptr),
mFbxLoader(nullptr),
mMesh(nullptr)
{
}

GraphicsClass::GraphicsClass(const GraphicsClass&)
{
}

GraphicsClass::~GraphicsClass()
{

}

bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
    bool result;

    mDirect = new D3DClass;
    if (mDirect == nullptr)
    {
        return false;
    }

    result = mDirect->Initialize(screenWidth,screenHeight,VSYNC_ENABLED,hwnd,FULL_SCREEN,SCREEN_DEPTH,SCREEN_NEAR);
    if (result == false)
    {
		MessageBox(hwnd, L"Could not initialize Direct3D.", L"Error", MB_OK);
		return false;
    }

    mFbxLoader = new FBXLoader;
    if (mFbxLoader == nullptr)
    {
        return false;
    }

    //mFbxLoader->LoadAnimation("Model\\Capoeira.fbx");
    mMesh = mFbxLoader->LoadFbx("Model\\Capoeira (1).fbx");
    result = mMesh->Initialize(mDirect->GetDevice());
    if (result == false)
    {
        return false;
    }

    mLight = new Light;
    if (mLight == nullptr)
    {
        return false;
    }

    mLight->SetAmbientColor(0.1f,0.1f,0.1f,1.0f);
    mLight->SetDiffuseColor(1.0f,1.0f,1.0f,1.0f);
    mLight->SetPosition(-5.0f,0.0f,-200.0f);
    mLight->SetSpecularColor(1.0f,1.0f,1.0f,1.0f);
    mLight->SetSpecularPower(50.0f);

    mCamera = new Camera;
    mCamera->SetPosition(0.0f,0.0f,-300.0f);
    mCamera->Render();

    mShader = new SkinnedMeshShader;
    result = mShader->Initialize(mDirect->GetDevice(),hwnd);
    if (result == false)
    {
        return false;
    }

    mTexture = new Texture;
    result = mTexture->Initialize(mDirect->GetDevice(),mDirect->GetDeviceContext(),"Texture\\CharacterTexture.dds");
    if (result == false)
    {
        return false;
    }

    return true;
}

void GraphicsClass::Shutdown()
{
    if (mDirect)
    {
        mDirect->Shutdown();
        delete mDirect;
        mDirect = nullptr;
    }

    if (mFbxLoader)
    {
        delete mFbxLoader;
        mFbxLoader = nullptr;
    }

    if (mMesh)
    {
        mMesh->Shutdown();
        delete mMesh;
        mMesh = nullptr;
    }

    if (mCamera)
    {
        delete mCamera;
        mCamera = nullptr;
    }

    if (mShader)
    {
        mShader->Shutdown();
        delete mShader;
        mShader = nullptr;
    }

    if (mTexture)
    {
        mTexture->Shutdown();
        delete mTexture;
        mTexture = nullptr;
    }
}

bool GraphicsClass::Frame()
{
    bool result;

    result = Render();
    if (result == false)
    {
        return false;
    }

    return true;
}

bool GraphicsClass::Render()
{
    bool result;
	XMMATRIX worldMatrix, worldMatrix_2D, viewMatrix, projectionMatrix, orthoMatrix;

    mDirect->BeginScene(0.0f,0.0f,0.0f,1.0f);

	mCamera->Render();

    mDirect->GetWorldMatrix(worldMatrix);
    mDirect->GetWorldMatrix(worldMatrix_2D);
    mDirect->GetProjectionMatrix(projectionMatrix);
    mDirect->GetOrthoMatrix(orthoMatrix);
    mCamera->GetViewMatrix(viewMatrix);

    mMesh->UpdateAnimation(ApplicationHandle->DeltaTime());
    mMesh->Render(mDirect->GetDeviceContext());

    mShader->Render(mDirect->GetDeviceContext(),mMesh->GetIndexCount(),worldMatrix,viewMatrix,projectionMatrix,mTexture->GetTexture(),mLight->GetPosition(),mLight->GetDiffuseColor(),mLight->GetAmbientColor(),mCamera->GetPosition(),mLight->GetSpecularColor(),mLight->GetSpecularPower(),mMesh->GetBoneTransform());
    

    //2D ·»´õ¸µ
    mDirect->TurnZBufferOff();
    mDirect->TurnOnAlphaBlending();

    mDirect->TurnOffAlphaBlending();
    mDirect->TurnZBufferOn();
    //2D ·»´õ¸µ ³¡

    mDirect->EndScene();
    
    return true;
}
