#include "GraphicsClass.h"
#include"SystemClass.h"

extern SystemClass* ApplicationHandle;

GraphicsClass::GraphicsClass():
mDirect(nullptr),
mFbxLoader(nullptr),
mMesh(nullptr),
mTexture(nullptr),
mCurrentRenderMesh(""),
mObject(nullptr)
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

    

    /*Mesh* mesh = mFbxLoader->LoadFbx("Model\\character.fbx");
    result = mesh->Initialize(mDirect->GetDevice());
    if (result == false)
    {
        return false;
    }*/

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

    mSpecularShader = new SpecularShaderClass;
    result = mSpecularShader->Initialize(mDirect->GetDevice(), hwnd);
    if (result == false)
    {
        return false;
    }

    mGrid = new GridClass;
    result = mGrid->Initialize(mDirect->GetDevice());
    if (result == false)
    {
        return false;
    }

    mColorShader = new ColorShader;
    result = mColorShader->Initialize(mDirect->GetDevice(), hwnd);
    if (result == false)
    {
        return false;
    }

    /*Texture* texture = new Texture;
    result = texture->Initialize(mDirect->GetDevice(),mDirect->GetDeviceContext(),"Texture\\map.dds");
    if (result == false)
    {
        return false;
    }

    mTexture = new Texture;
    result = mTexture->Initialize(mDirect->GetDevice(), mDirect->GetDeviceContext(), "Texture\\map.dds");
    if (result == false)
    {
        return false;
    }*/

    /*mObject = new GameObject;
    mObject->SetMesh(mesh);
    mObject->SetTexture(texture);*/
    /*Mesh* mesh = mFbxLoader->LoadFbx("Model\\character.fbx");
    mesh->Initialize(mDirect->GetDevice());
    mObject->SetMesh(mesh);
    Texture* texture = new Texture;
    result = texture->Initialize(mDirect->GetDevice(), mDirect->GetDeviceContext(), "Texture\\CharacterTexture.dds");
    if (result == false)
    {
        return false;
    }
    mObject->SetTexture(texture);*/
    mImgui = new ImguiClass;
    mImgui->Initialize(hwnd, mDirect->GetDevice(), mDirect->GetDeviceContext());

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

    if (mSpecularShader)
    {
        mSpecularShader->Shutdown();
        delete mSpecularShader;
        mSpecularShader = nullptr;
    }

    if (mTexture)
    {
        mTexture->Shutdown();
        delete mTexture;
        mTexture = nullptr;
    }

    if (mObject)
    {
        mObject->Shutdown();
        delete mObject;
        mObject = nullptr;
    }

    if (mGrid)
    {
        mGrid->Shutdown();
        delete mGrid;
        mGrid = nullptr;
    }

    if (mColorShader)
    {
        mColorShader->Shutdown();
        delete mColorShader;
        mColorShader = nullptr;
    }
}

bool GraphicsClass::Frame()
{
    bool result;
    static float speed = 0.2f;

    if (InputClass::GetInstance()->IsMouse1Pressed() == true)
    {
        //mObject->Update(ApplicationHandle->DeltaTime());
        if (InputClass::GetInstance()->IsWPressed() == true)
        {
            XMVECTOR forward = XMLoadFloat3(&mCamera->GetTransform().Forward());
            XMFLOAT3 delta;
            XMStoreFloat3(&delta, forward * speed * ApplicationHandle->DeltaTime());
            mCamera->GetTransform().Translate(delta);
        }
        if (InputClass::GetInstance()->IsSPressed() == true)
        {
            XMVECTOR back = -XMLoadFloat3(&mCamera->GetTransform().Forward());
            XMFLOAT3 delta;
            XMStoreFloat3(&delta, back * speed * ApplicationHandle->DeltaTime());
            mCamera->GetTransform().Translate(delta);
        }
        if (InputClass::GetInstance()->IsAPressed() == true)
        {
            XMVECTOR left = -XMLoadFloat3(&mCamera->GetTransform().Right());
            XMFLOAT3 delta;
            XMStoreFloat3(&delta, left * speed * ApplicationHandle->DeltaTime());
            mCamera->GetTransform().Translate(delta);
        }
        if (InputClass::GetInstance()->IsDPressed() == true)
        {
            XMVECTOR right = XMLoadFloat3(&mCamera->GetTransform().Right());
            XMFLOAT3 delta;
            XMStoreFloat3(&delta, right * speed * ApplicationHandle->DeltaTime());
            mCamera->GetTransform().Translate(delta);
        }

        /*if (InputClass::GetInstance()->IsUpArrowPressed() == true)
        {
            mCamera->GetTransform().Rotate(-speed * ApplicationHandle->DeltaTime(), Axis::Xaxis);
        }
        if (InputClass::GetInstance()->IsDownArrowPressed() == true)
        {
            mCamera->GetTransform().Rotate(speed * ApplicationHandle->DeltaTime(), Axis::Xaxis);
        }
        if (InputClass::GetInstance()->IsRightArrowPressed() == true)
        {
            mCamera->GetTransform().Rotate(speed * ApplicationHandle->DeltaTime(), Axis::Yaxis);
        }
        if (InputClass::GetInstance()->IsLeftArrowPressed() == true)
        {
            mCamera->GetTransform().Rotate(-speed * ApplicationHandle->DeltaTime(), Axis::Yaxis);
        }*/

        mCamera->GetTransform().Rotate(InputClass::GetInstance()->GetMouseX() * ApplicationHandle->DeltaTime(), Axis::Yaxis);
        mCamera->GetTransform().Rotate(InputClass::GetInstance()->GetMouseY() * ApplicationHandle->DeltaTime(), Axis::Xaxis);

    }

    if (mCurrentRenderMesh != "")
    {
        meshMap[mCurrentRenderMesh].Update(ApplicationHandle->DeltaTime());
    }

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

    static XMMATRIX gridWorldMatrix = XMMatrixScaling(50.0f, 50.0f, 50.0f);

    //±×¸®µå ·»´õ¸µ
    mGrid->Render(mDirect->GetDeviceContext());
    mColorShader->Render(mDirect->GetDeviceContext(), mGrid->GetIndexCount(), gridWorldMatrix, viewMatrix, projectionMatrix);

    if (mCurrentRenderMesh != "")
    {
        GameObject& renderObject = meshMap[mCurrentRenderMesh];
        renderObject.Render(mDirect->GetDeviceContext());
        if(renderObject.IsHasAnimation())
            mShader->Render(mDirect->GetDeviceContext(), renderObject.GetIndexCount(), renderObject.Transfrom().GetTransform(), viewMatrix, projectionMatrix, renderObject.GetTexture(), mLight->GetPosition(), mLight->GetDiffuseColor(), mLight->GetAmbientColor(), mCamera->GetPosition(), mLight->GetSpecularColor(), mLight->GetSpecularPower(), renderObject.GetBoneTransform());
        else
            mSpecularShader->Render(mDirect->GetDeviceContext(), renderObject.GetIndexCount(), renderObject.Transfrom().GetTransform(), viewMatrix, projectionMatrix, renderObject.GetTexture(), mLight->GetPosition(), mLight->GetDiffuseColor(), mLight->GetAmbientColor(), mCamera->GetPosition(), mLight->GetSpecularColor(), mLight->GetSpecularPower());
    }
   /* mObject->Render(mDirect->GetDeviceContext());
    mShader->Render(mDirect->GetDeviceContext(), mObject->GetIndexCount(), mObject->Transfrom().GetTransform(), viewMatrix, projectionMatrix, mObject->GetTexture(), mLight->GetPosition(), mLight->GetDiffuseColor(), mLight->GetAmbientColor(), mCamera->GetPosition(), mLight->GetSpecularColor(), mLight->GetSpecularPower(), mObject->GetBoneTransform());*/

    //2D ·»´õ¸µ
    mDirect->TurnZBufferOff();
    mDirect->TurnOnAlphaBlending();

    mDirect->TurnOffAlphaBlending();
    mDirect->TurnZBufferOn();
    //2D ·»´õ¸µ ³¡

    bool loadFbx = false;
    fs::path loadPath;
    //IMGUI ·»´õ¸µ
    mImgui->Render(&loadFbx,loadPath, meshMap[mCurrentRenderMesh].Transfrom(),meshMap,mCurrentRenderMesh,*mCamera,mLight);

    if (loadFbx == true)
    {
        std::string meshKey = loadPath.filename().string();
        GameObject loadObject;
        Mesh* loadMesh = mFbxLoader->LoadFbx(const_cast<char*>(loadPath.string().c_str()));
        bool result = loadMesh->Initialize(mDirect->GetDevice());
        if (result == false)
        {
            return false;
        }
        loadObject.SetMesh(loadMesh);
        Texture* texture = new Texture;
        result = texture->Initialize(mDirect->GetDevice(), mDirect->GetDeviceContext(), "Texture\\vanguard_diffuse.dds");
        if (result == false)
        {
            return false;
        }
        loadObject.SetTexture(texture);

        meshMap.insert(std::pair<std::string, GameObject>(meshKey, loadObject));

    }

    mDirect->EndScene();
    
    return true;
}
