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

    mMesh = mFbxLoader->LoadFbx("Model\\character.fbx");
    result = mMesh->Initialize(mDirect->GetDevice());
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

    mDirect->GetWorldMatrix(worldMatrix);
    mDirect->GetWorldMatrix(worldMatrix_2D);
    mDirect->GetProjectionMatrix(projectionMatrix);
    mDirect->GetOrthoMatrix(orthoMatrix);

    //2D ·»´õ¸µ
    mDirect->TurnZBufferOff();
    mDirect->TurnOnAlphaBlending();

    mDirect->TurnOffAlphaBlending();
    mDirect->TurnZBufferOn();
    //2D ·»´õ¸µ ³¡

    mDirect->EndScene();
    ApplicationHandle->DeltaTime();
    return true;
}
