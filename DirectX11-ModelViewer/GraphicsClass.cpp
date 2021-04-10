#include "GraphicsClass.h"
#include"SystemClass.h"

extern SystemClass* ApplicationHandle;

GraphicsClass::GraphicsClass():
mDirect(nullptr),
mFbxLoader(nullptr),
mMesh(nullptr),
mTexture(nullptr),
mCurrentRenderMesh(""),
mObject(nullptr),
mRenderTexture(nullptr),
mDepthShader(nullptr),
mShadowShader(nullptr),
mGroundModel(nullptr),
mSolidShader(nullptr),
mArrowModel(nullptr)
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

    mScreenWidth = screenWidth;
    mScreenHeight = screenHeight;

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
    mLight->SetPosition(-5.0f,10.0f,-200.0f);
    mLight->SetSpecularColor(1.0f,1.0f,1.0f,1.0f);
    mLight->SetSpecularPower(50.0f);
    mLight->GenerateProjectionMatrix(SCREEN_DEPTH, SCREEN_NEAR);

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

    mRenderTexture = new RenderTextureClass;
    result = mRenderTexture->Initialize(mDirect->GetDevice(), SHADOWMAP_WIDTH, SHADOWMAP_HEIGHT, SCREEN_DEPTH, SCREEN_NEAR);
    if (result == false)
    {
        return false;
    }

    mDepthShader = new DepthShaderClass;
    result = mDepthShader->Initialize(mDirect->GetDevice(), hwnd);
    if (result == false)
    {
        return false;
    }

    mShadowShader = new ShadowShaderClass;
    result = mShadowShader->Initialize(mDirect->GetDevice(), hwnd);
    if (result == false)
    {
        return false;
    }
    
    mGroundModel = new ModelClass;
    result = mGroundModel->Initialize(mDirect->GetDevice(), "Model\\plane01.txt");
    if (result == false)
    {
        return false;
    }
    mGroundTexture = new Texture;
    mGroundTexture->Initialize(mDirect->GetDevice(), mDirect->GetDeviceContext(), "Texture\\White.dds");
    mGroundModel->SetTexture(mGroundTexture);
    mGroundModel->SetPosition(0.f, 0.f, 0.f);

    mGroundMesh = mFbxLoader->LoadFbx("Model\\floor.fbx");
    result = mGroundMesh->Initialize(mDirect->GetDevice());
    if (result == false)
    {
        return false;
    }

    mDirect->GetWorldMatrix(floorWorld);
    floorWorld *= XMMatrixRotationRollPitchYaw(XMConvertToRadians(90.0f), 0.0f, 0.0f);

    mSkinnedDepthShader = new SkinnedDepthShaderClass;
    result = mSkinnedDepthShader->Initialize(mDirect->GetDevice(), hwnd);
    if (result == false)
    {
        return false;
    }

    mSolidShader = new SolidColorShader;
    result = mSolidShader->Initialize(mDirect->GetDevice(), hwnd, XMFLOAT4(1.0f,0.0f,0.0f,1.0f));
    if (result == false)
        return false;

    mArrowModel = new ArrowModel;
    result = mArrowModel->Initialize(mDirect->GetDevice());
    if (result == false)
        return false;

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

    if (mRenderTexture)
    {
        mRenderTexture->Shutdown();
        delete mRenderTexture;
        mRenderTexture = nullptr;
    }

    if (mDepthShader)
    {
        mDepthShader->Shutdown();
        delete mDepthShader;
        mDepthShader = nullptr;
    }

    if (mShadowShader)
    {
        mShadowShader->Shutdown();
        delete mShadowShader;
        mShadowShader = nullptr;
    }

    if (mGroundModel)
    {
        mGroundModel->Shutdown();
        delete mGroundModel;
        mGroundModel = nullptr;
    }

    if (mGroundMesh)
    {
        mGroundMesh->Shutdown();
        delete mGroundMesh;
        mGroundMesh = nullptr;
    }

    if (mGroundTexture)
    {
        mGroundTexture->Shutdown();
        delete mGroundTexture;
        mGroundTexture = nullptr;
    }

    if (mSkinnedDepthShader)
    {
        mSkinnedDepthShader->Shutdown();
        delete mSkinnedDepthShader;
        mSkinnedDepthShader = nullptr;
    }

    if (mSolidShader)
    {
        mSolidShader->Shutdown();
        delete mSolidShader;
        mSolidShader = nullptr;
    }

    if (mArrowModel)
    {
        mArrowModel->Shutdown();
        delete mArrowModel;
        mArrowModel = nullptr;
    }
}

bool GraphicsClass::Frame()
{
    bool result;
    static float speed = 0.2f;
    int mouseX = 0;
    int mouseY = 0;

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

        if (InputClass::GetInstance()->IsMouse0Pressed())
        {
            InputClass::GetInstance()->GetMouseLocation(mouseX, mouseY);
            TestIntersection(mouseX, mouseY, meshMap[mCurrentRenderMesh].Transfrom().GetPosition());
        }
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
    XMMATRIX lightViewMatrix, lightProjectionMatrix;

    result = RenderSceneToTexture();
    if (result == false)
    {
        return false;
    }

    mDirect->BeginScene(0.0f,0.0f,0.0f,1.0f);

	mCamera->Render();

    mLight->GenerateViewMatrix();

    mDirect->GetWorldMatrix(worldMatrix);
    mDirect->GetWorldMatrix(worldMatrix_2D);
    mDirect->GetProjectionMatrix(projectionMatrix);
    mDirect->GetOrthoMatrix(orthoMatrix);
    mCamera->GetViewMatrix(viewMatrix);

    mLight->GetViewMatrix(lightViewMatrix);
    mLight->GetProjectionMatrix(lightProjectionMatrix);

    static XMMATRIX gridWorldMatrix = XMMatrixScaling(50.0f, 50.0f, 50.0f);

    //±×¸®µå ·»´õ¸µ
    mGrid->Render(mDirect->GetDeviceContext());
    mColorShader->Render(mDirect->GetDeviceContext(), mGrid->GetIndexCount(), gridWorldMatrix, viewMatrix, projectionMatrix);

    if (mCurrentRenderMesh != "")
    {
        GameObject& renderObject = meshMap[mCurrentRenderMesh];
        renderObject.Render(mDirect->GetDeviceContext());
        if (renderObject.IsHasAnimation())
        {
            mShader->Render(mDirect->GetDeviceContext(), renderObject.GetIndexCount(), renderObject.Transfrom().GetTransform(), viewMatrix, projectionMatrix, renderObject.GetTexture(), mLight->GetPosition(), mLight->GetDiffuseColor(), mLight->GetAmbientColor(), mCamera->GetPosition(), mLight->GetSpecularColor(), mLight->GetSpecularPower(), renderObject.GetBoneTransform());
            //mShadowShader->Render(mDirect->GetDeviceContext(), renderObject.GetIndexCount(), renderObject.Transfrom().GetTransform(), viewMatrix, projectionMatrix, lightViewMatrix, lightProjectionMatrix, renderObject.GetTexture(), mRenderTexture->GetShaderResourceView(), mLight->GetPosition(), mLight->GetAmbientColor(), mLight->GetDiffuseColor());
            //mDepthShader->Render(mDirect->GetDeviceContext(), renderObject.GetIndexCount(), renderObject.Transfrom().GetTransform(), viewMatrix, projectionMatrix);
        }
        else
        {
            mSpecularShader->Render(mDirect->GetDeviceContext(), renderObject.GetIndexCount(), renderObject.Transfrom().GetTransform(), viewMatrix, projectionMatrix, renderObject.GetTexture(), mLight->GetPosition(), mLight->GetDiffuseColor(), mLight->GetAmbientColor(), mCamera->GetPosition(), mLight->GetSpecularColor(), mLight->GetSpecularPower());
        }

        mArrowModel->Render(mDirect->GetDeviceContext());
        mSolidShader->Render(mDirect->GetDeviceContext(), mArrowModel->GetIndexCount(),   XMMatrixScaling(20.0f,20.0f,100.0f) , viewMatrix, projectionMatrix);
    }
   /* mObject->Render(mDirect->GetDeviceContext());
    mShader->Render(mDirect->GetDeviceContext(), mObject->GetIndexCount(), mObject->Transfrom().GetTransform(), viewMatrix, projectionMatrix, mObject->GetTexture(), mLight->GetPosition(), mLight->GetDiffuseColor(), mLight->GetAmbientColor(), mCamera->GetPosition(), mLight->GetSpecularColor(), mLight->GetSpecularPower(), mObject->GetBoneTransform());*/

    /*float posX, posY, posZ;
    mDirect->GetWorldMatrix(worldMatrix);
    mGroundModel->GetPosition(posX, posY, posZ);
    worldMatrix = XMMatrixScaling(100.f, 0.0f, 100.f) * XMMatrixTranslation(posX, posY, posZ);

    mGroundModel->Render(mDirect->GetDeviceContext());
    mShadowShader->Render(mDirect->GetDeviceContext(), mGroundModel->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, lightViewMatrix, lightProjectionMatrix, mGroundModel->GetTexture(), mRenderTexture->GetShaderResourceView(), mLight->GetPosition(), mLight->GetAmbientColor(), mLight->GetDiffuseColor());*/
    //mDepthShader->Render(mDirect->GetDeviceContext(), mGroundModel->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix);

    mDirect->GetWorldMatrix(worldMatrix);
    mGroundMesh->Render(mDirect->GetDeviceContext());
    mShadowShader->Render(mDirect->GetDeviceContext(), mGroundMesh->GetIndexCount(), floorWorld, viewMatrix, projectionMatrix, lightViewMatrix, lightProjectionMatrix,mGroundTexture->GetTexture(), mRenderTexture->GetShaderResourceView(), mLight->GetPosition(), mLight->GetAmbientColor(), mLight->GetDiffuseColor());

    //2D ·»´õ¸µ
    mDirect->TurnZBufferOff();
    mDirect->TurnOnAlphaBlending();

    mDirect->TurnOffAlphaBlending();
    mDirect->TurnZBufferOn();
    //2D ·»´õ¸µ ³¡

    bool loadFbx = false;
    fs::path loadPath;
    //IMGUI ·»´õ¸µ
    mImgui->Render(&loadFbx,loadPath, meshMap[mCurrentRenderMesh].Transfrom(),meshMap,mCurrentRenderMesh,*mCamera,mLight,mCurrentRenderMesh == "" ? nullptr : &(meshMap[mCurrentRenderMesh]));

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

bool GraphicsClass::RenderSceneToTexture()
{
    bool result;
    XMMATRIX worldMatrix, lightViewMatrix, lightProjectionMatrix;

    float posX = 0.f;
    float posY = 0.f;
    float posZ = 0.f;

    mRenderTexture->SetRenderTarget(mDirect->GetDeviceContext());

    mRenderTexture->ClearRenderTarget(mDirect->GetDeviceContext(), 0.0f, 0.f, 0.f, 1.f);

    mLight->GenerateViewMatrix();

    mDirect->GetWorldMatrix(worldMatrix);

    mLight->GetViewMatrix(lightViewMatrix);
    mLight->GetProjectionMatrix(lightProjectionMatrix);
    
    if (mCurrentRenderMesh != "")
    {
        GameObject& renderObject = meshMap[mCurrentRenderMesh];
        worldMatrix = renderObject.Transfrom().GetTransform();

        renderObject.Render(mDirect->GetDeviceContext());
        if (renderObject.IsHasAnimation())
        {
            result = mSkinnedDepthShader->Render(mDirect->GetDeviceContext(), renderObject.GetIndexCount(), worldMatrix, lightViewMatrix, lightProjectionMatrix,renderObject.GetBoneTransform());
            if (result == false)
            {
                return false;
            }
        }
        else
        {
            result = mDepthShader->Render(mDirect->GetDeviceContext(), renderObject.GetIndexCount(), worldMatrix, lightViewMatrix, lightProjectionMatrix);
            if (result == false)
            {
                return false;
            }
        }
    }

    mDirect->GetWorldMatrix(worldMatrix);

    /*mGroundModel->GetPosition(posX, posY, posZ);
    worldMatrix = XMMatrixScaling(100.f, 0.0f, 100.f) * XMMatrixTranslation(posX, posY, posZ);

    mGroundModel->Render(mDirect->GetDeviceContext());
    result = mDepthShader->Render(mDirect->GetDeviceContext(), mGroundModel->GetIndexCount(), worldMatrix, lightViewMatrix, lightProjectionMatrix);
    if (result == false)
    {
        return false;
    }*/

    mGroundMesh->Render(mDirect->GetDeviceContext());
    result = mDepthShader->Render(mDirect->GetDeviceContext(), mGroundMesh->GetIndexCount(), floorWorld, lightViewMatrix, lightProjectionMatrix);
    if (result == false)
    {
        return false;
    }

    mDirect->SetBackBufferRenderTarget();

    mDirect->ResetViewport();

    return true;
}

void GraphicsClass::TestIntersection(int mouseX, int mouseY, XMFLOAT3 position)
{
    XMMATRIX projectionMatrix, viewMatrix, inverseViewMatrix, inverseWorldMatrix , worldMatrix;
    XMFLOAT3 direction, origin, rayOrigin, rayDirection;

    float pointX = ((2.0f * (float)mouseX) / (float)mScreenWidth) - 1.0f;
    float pointY = (((2.0f * (float)mouseY) / (float)mScreenHeight) - 1.0f) * -1.0f;

    mDirect->GetProjectionMatrix(projectionMatrix);

    XMFLOAT3X3 projectionMatrix4;
    XMStoreFloat3x3(&projectionMatrix4, projectionMatrix);

    pointX = pointX / projectionMatrix4._11;
    pointY = pointY / projectionMatrix4._22;

    mCamera->GetViewMatrix(viewMatrix);
    inverseViewMatrix = XMMatrixInverse(nullptr, viewMatrix);

    XMFLOAT3X3 inverseViewMatrix4;
    XMStoreFloat3x3(&inverseViewMatrix4, inverseViewMatrix);

    direction.x = (pointX * inverseViewMatrix4._11) + (pointY * inverseViewMatrix4._21) + inverseViewMatrix4._31;
    direction.y = (pointX * inverseViewMatrix4._12) + (pointY * inverseViewMatrix4._22) + inverseViewMatrix4._32;
    direction.z = (pointX * inverseViewMatrix4._13) + (pointY * inverseViewMatrix4._23) + inverseViewMatrix4._33;

    origin = mCamera->GetPosition();

    mDirect->GetWorldMatrix(worldMatrix);
    XMMATRIX translateMatrix = XMMatrixTranslation(position.x, position.y, position.z);
    worldMatrix = XMMatrixMultiply(worldMatrix,translateMatrix);

    inverseWorldMatrix = XMMatrixInverse(nullptr, worldMatrix);

    XMStoreFloat3(&rayOrigin, XMVector3TransformCoord(XMVectorSet(origin.x, origin.y, origin.z, 0.f),inverseWorldMatrix));
    XMStoreFloat3(&direction, XMVector3TransformNormal(XMVectorSet(direction.x, direction.y, direction.z, 0.f), inverseWorldMatrix));

    XMStoreFloat3(&rayDirection, XMVector3Normalize(XMVectorSet(direction.x, direction.y, direction.z, 0.f)));

    bool result = mArrowModel->RayIntersect(rayOrigin, rayDirection, position, XMFLOAT3(20.0f, 20.0f, 100.0f));
    if (result == true)
    {
        result = true;
    }
}
