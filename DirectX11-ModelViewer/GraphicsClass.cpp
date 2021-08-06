#include "GraphicsClass.h"
#include"SystemClass.h"
#include "Component.h"
#include "TransformComponent.h"
#include "MeshRenderComponent.h"
#include "SkinnedMeshRenderComponent.h"
#include "SkinnedMeshBumpRenderComponent.h"
#include "AnimatorComponent.h"
#include "GameObjectBrowser.h"

extern SystemClass* ApplicationHandle;
extern Camera* gMainCamera;
extern Light* gMainLight;
extern D3DClass* gDirect;

GraphicsClass::GraphicsClass() :
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
    mForwardArrowModel(nullptr),
    mGameObject(nullptr)
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

    gDirect = mDirect;

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
    gMainLight = mLight;

    mCamera = new Camera;
    mCamera->SetPosition(0.0f,0.0f,-300.0f);
    mCamera->Render();
    mCamera->GetViewMatrix(mBaseViewMatrix);
    gMainCamera = mCamera;

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

    mForwardArrowModel = new ArrowModel;
    result = mForwardArrowModel->Initialize(mDirect->GetDevice(),ArrowDirection::Forward,XMFLOAT3(20.0f,20.0f,100.0f));
    if (result == false)
        return false;

    mRightArrowModel = new ArrowModel;
    result = mRightArrowModel->Initialize(mDirect->GetDevice(), ArrowDirection::Right, XMFLOAT3(100.0f, 20.0f, 20.0f));
    if (result == false)
        return false;

    mUpArrowModel = new ArrowModel;
    result = mUpArrowModel->Initialize(mDirect->GetDevice(), ArrowDirection::Up, XMFLOAT3(20.0f, 100.0f, 20.0f));
    if (result == false)
        return false;

    mMouseBitmap = new BitmapClass;
    result = mMouseBitmap->Initialize(mDirect->GetDevice(), mDirect->GetDeviceContext(), screenWidth, screenHeight, "Texture/mouse.dds", 32, 32);
    if (result == false)
        return false;

    mTextureShader = new TextureShaderClass;
    result = mTextureShader->Initialize(mDirect->GetDevice(),hwnd);
    if (result == false)
        return false;

    mSkinnedBumpShader = new SkinnedBumpShader;
    result = mSkinnedBumpShader->Initialize(mDirect->GetDevice(), hwnd);
    if (result == false)
        return false;

   /* mGameObject = GameObjectClass::Create();
    mGameObject->InsertComponent(new TransformComponent);*/

    ifstream in; //읽기 스트림 생성
    SkinnedMeshData loadMesh; //받을 객체 생성
    in.open("Character.SM", ios_base::binary); //바이너리모드로 파일을 열었습니다.
    boost::archive::binary_iarchive in_archive(in); //연 스트림을 넘겨주어서 직렬화객체 초기화
    in_archive >> loadMesh; //읽기
    in.close();

    ifstream in2;
    SaveAnimationData loadAnim; //받을 객체 생성
    in2.open("Test.Animation", ios_base::binary); //바이너리모드로 파일을 열었습니다.
    boost::archive::binary_iarchive in_archive2(in2); //연 스트림을 넘겨주어서 직렬화객체 초기화
    in_archive2 >> loadAnim; //읽기
    in2.close();

    anim = new AnimationData;
    *anim = loadAnim;

    /*mTempMesh = new TempMesh;
    mTempMesh->SetMesh(loadMesh);

    if (mTempMesh->InitializeBuffer(mDirect->GetDevice()) == false)
        return false;*/

    mCharacterTexture = new Texture;
    if (mCharacterTexture->Initialize(mDirect->GetDevice(), mDirect->GetDeviceContext(), "Texture\\vanguard_diffuse.png") == false)
        return false;

    mCharacterMesh = new MeshClass;
    mCharacterMesh->operator=(loadMesh);
    mCharacterMesh->Initalize(mDirect->GetDevice());

    /*MeshRenderComponent* rendercomp = new MeshRenderComponent();
    rendercomp->Initalize(mCharacterMesh, mSpecularShader, mCharacterTexture);*/

    /*SkinnedMeshRenderComponent* rendercomp = new SkinnedMeshRenderComponent();
    rendercomp->Initalize(mCharacterMesh, mShader, mSkinnedDepthShader,mCharacterTexture);

    mGameObject->InsertComponent(rendercomp);
    
    AnimatorComponent* animComp = new AnimatorComponent();
    animComp->Play();
    animComp->SetAnimation(anim);

    mGameObject->InsertComponent(animComp);*/

    /*dummyBone.resize(120);
    for (auto& m : dummyBone)
    {
        m = XMMatrixTranspose(XMMatrixIdentity());
    }*/

    gameObjectBrowser = new GameObjectBrowser;

    AssetClass::Initialize(mDirect->GetDevice(), mDirect->GetDeviceContext());

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

    if (mForwardArrowModel)
    {
        mForwardArrowModel->Shutdown();
        delete mForwardArrowModel;
        mForwardArrowModel = nullptr;
    }

    if (mMouseBitmap)
    {
        mMouseBitmap->Shutdown();
        delete mMouseBitmap;
        mMouseBitmap = nullptr;
    }

    if (mTextureShader)
    {
        mTextureShader->Shutdown();
        delete mTextureShader;
        mTextureShader = nullptr;
    }

    if (mSkinnedBumpShader)
    {
        mSkinnedBumpShader->Shutdown();
        delete mSkinnedBumpShader;
        mSkinnedBumpShader = nullptr;
    }

    if (mCharacterTexture)
    {
        mCharacterTexture->Shutdown();
        delete mCharacterTexture;
        mCharacterTexture = nullptr;
    }

    if (mCharacterMesh)
    {
        mCharacterMesh->Shutdown();
        delete mCharacterMesh;
        mCharacterMesh = nullptr;
    }

    if (mGameObject)
    {
        mGameObject->Destroy();
        delete mGameObject;
        mGameObject = nullptr;
    }

    if (anim)
    {
        delete anim;
        anim = nullptr;
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

        if (mCurrentPositionGizumoState == PositionGizumoState::NONE)
        {
            if (InputClass::GetInstance()->IsMouse0Pressed())
            {
                InputClass::GetInstance()->GetWMMouseLocation(mouseX, mouseY);
                TestIntersection(mouseX, mouseY, meshMap[mCurrentRenderMesh].Transfrom().GetPosition());
            }
        }
        else
        {
            XMVECTOR cameraForward;
            XMVECTOR arrowDirection;
            float yWeight = 0.0f;
            float xWeight = 0.0f;

            switch (mCurrentPositionGizumoState)
            {
            case PositionGizumoState::FORWARD:
            {
                cameraForward = XMLoadFloat3(&mCamera->GetTransform().Forward());
                arrowDirection = XMVectorSet(0.f, 0.f, 1.f, 0.f);
                XMVECTOR dot = XMVector3Dot(cameraForward, arrowDirection);
                yWeight = XMVectorGetX(dot);
                xWeight = 1.0f - abs(yWeight);
                float deltaWeight = (ApplicationHandle->Input().GetMouseY() * yWeight) + (ApplicationHandle->Input().GetMouseX() * xWeight);
                meshMap[mCurrentRenderMesh].Transfrom().Translate(XMFLOAT3(0.f, 0.f, 1.f * deltaWeight));
            }

                break;
            case PositionGizumoState::RIGHT:
            {
                cameraForward = XMLoadFloat3(&mCamera->GetTransform().Forward());
                arrowDirection = XMVectorSet(1.f, 0.f, 0.f, 0.f);
                XMVECTOR dot = XMVector3Dot(cameraForward, arrowDirection);
                yWeight = XMVectorGetX(dot);
                xWeight = 1.0f - abs(yWeight);
                float deltaWeight = (ApplicationHandle->Input().GetMouseY() * yWeight) + (ApplicationHandle->Input().GetMouseX() * xWeight);
                meshMap[mCurrentRenderMesh].Transfrom().Translate(XMFLOAT3(1.f* deltaWeight, 0.f, 0.f));

            }
                break;
            case PositionGizumoState::UP:
            {
                cameraForward = XMLoadFloat3(&mCamera->GetTransform().Forward());
                arrowDirection = XMVectorSet(0.f, 1.f, 0.f, 0.f);
                float deltaWeight = -(ApplicationHandle->Input().GetMouseY());
                meshMap[mCurrentRenderMesh].Transfrom().Translate(XMFLOAT3(0.f, 1.f * deltaWeight, 0.f ));
            }
                break;
            }

            

            if (InputClass::GetInstance()->IsMouse0Pressed() == false)
            {
                mCurrentPositionGizumoState = PositionGizumoState::NONE;
            }
        }
    }

    //mGameObject->LateUpdate(ApplicationHandle->DeltaTime());

    for (const auto& gameObject : mGameObejcts)
    {
        gameObject.second->Update(ApplicationHandle->DeltaTime());
    }

    for (const auto& gameObject : mGameObejcts)
    {
        gameObject.second->LateUpdate(ApplicationHandle->DeltaTime());
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

    //그리드 렌더링
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

        XMMATRIX transformMatrix;
        XMFLOAT3 position = renderObject.Transfrom().GetPosition();
        transformMatrix = XMMatrixTranslation(position.x, position.y, position.z);

        mForwardArrowModel->Render(mDirect->GetDeviceContext());
        mSolidShader->Render(mDirect->GetDeviceContext(), mForwardArrowModel->GetIndexCount(), transformMatrix  , viewMatrix, projectionMatrix, XMFLOAT4(0.0f, 0.0f, 1.f, 1.f));

        mRightArrowModel->Render(mDirect->GetDeviceContext());
        mSolidShader->Render(mDirect->GetDeviceContext(), mRightArrowModel->GetIndexCount(), transformMatrix , viewMatrix, projectionMatrix, XMFLOAT4(1.0f, 0.0f, 0.f, 1.f));

        mUpArrowModel->Render(mDirect->GetDeviceContext());
        mSolidShader->Render(mDirect->GetDeviceContext(), mUpArrowModel->GetIndexCount(), transformMatrix , viewMatrix, projectionMatrix, XMFLOAT4(0.0f, 1.f, 0.f, 1.f));
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

 /*   mTempMesh->Render(mDirect->GetDeviceContext());
    mShader->Render(mDirect->GetDeviceContext(), mTempMesh->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, mCharacterTexture->GetTexture(), mLight->GetPosition(), mLight->GetDiffuseColor(), mLight->GetAmbientColor(), mCamera->GetPosition(), mLight->GetSpecularColor(), mLight->GetSpecularPower(), dummyBone);*/

    mDirect->GetWorldMatrix(worldMatrix);
    mGroundMesh->Render(mDirect->GetDeviceContext());
    mShadowShader->Render(mDirect->GetDeviceContext(), mGroundMesh->GetIndexCount(), floorWorld, viewMatrix, projectionMatrix, lightViewMatrix, lightProjectionMatrix,mGroundTexture->GetTexture(), mRenderTexture->GetShaderResourceView(), mLight->GetPosition(), mLight->GetAmbientColor(), mLight->GetDiffuseColor());

    /*MeshRenderComponent* currentRenderer;
    currentRenderer = mGameObject->GetComponent<MeshRenderComponent>();

    if (currentRenderer != nullptr)
    {
        currentRenderer->Render(mDirect->GetDeviceContext());
    }*/

    for (const auto& gameObject : mGameObejcts)
    {
        MeshRenderComponent* renderComp = gameObject.second->GetComponent<MeshRenderComponent>();
        renderComp->Render(mDirect->GetDeviceContext());
    }

    //2D 렌더링
    mDirect->TurnZBufferOff();
    mDirect->TurnOnAlphaBlending();

    int mouseX = 0;
    int mouseY = 0;
    InputClass::GetInstance()->GetMouseLocation(mouseX, mouseY);

    int wmX, wmY;
    ApplicationHandle->Input().GetWMMouseLocation(wmX, wmY);

    if (mMouseBitmap->Render(mDirect->GetDeviceContext(), wmX, wmY) == false)
    {
        return false;
    }

    if (mTextureShader->Render(mDirect->GetDeviceContext(), mMouseBitmap->GetIndexCount(), worldMatrix, mBaseViewMatrix, orthoMatrix, mMouseBitmap->GetTexture()) == false)
    {
        return false;
    }

    mDirect->TurnOffAlphaBlending();
    mDirect->TurnZBufferOn();
    //2D 렌더링 끝

    bool loadFbx = false;
    bool loadAnim = false;
    fs::path loadPath;

    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    //IMGUI 렌더링
    mImgui->Render(&loadFbx,loadPath, meshMap[mCurrentRenderMesh].Transfrom(),meshMap,mCurrentRenderMesh,*mCamera,mLight,mCurrentRenderMesh == "" ? nullptr : &(meshMap[mCurrentRenderMesh]), wmX, wmY, &loadAnim, loadPath);

    if (loadFbx == true)
    {
        /*std::string meshKey = loadPath.filename().string();
        GameObject loadObject;
        Mesh* loadMesh = mFbxLoader->LoadFbx(const_cast<char*>(loadPath.string().c_str()));
        bool result = loadMesh->Initialize(mDirect->GetDevice());
        if (result == false)
        {
            return false;
        }
        loadObject.SetMesh(loadMesh);
        Texture* texture = new Texture;
        result = texture->Initialize(mDirect->GetDevice(), mDirect->GetDeviceContext(), "Texture\\vanguard_diffuse.png");
        if (result == false)
        {
            return false;
        }
        loadObject.SetTexture(texture);

        meshMap.insert(std::pair<std::string, GameObject>(meshKey, loadObject));*/
        mFbxLoader->LoadFbxFile(loadPath);
    }

    if (loadAnim == true)
    {
        mFbxLoader->LoadAnimation(const_cast<char*>(loadPath.string().c_str()));
    }

    //modelListBrowser.Render();
    static std::vector<std::string> gameObejctNames;
    gameObejctNames.clear();
    for (const auto& go : mGameObejcts)
    {
        gameObejctNames.push_back(go.first);
    }

    //gameObjectBrowser->Render(&addGameObject, gameObejctNames);

    static std::string currentGameObject;
    static bool addGameObject = false;
    static std::string selectModelKey;
    static std::string selectTextureKey;
    static std::string selectBumpTextureKey;
    static std::string selectSpecularTextureKey;
    static RendererType renderType = RendererType::ONLYSPECULAR;
    modelListBrowser.RenderGameObjectList(&addGameObject,&currentGameObject,&selectModelKey,&selectTextureKey, selectBumpTextureKey,selectSpecularTextureKey, gameObejctNames, renderType);

    if (addGameObject == true)
    {
        GameObjectClass* newGameObejct = GameObjectClass::Create();

        newGameObejct->InsertComponent(new TransformComponent());

        if (AssetClass::mMeshMap[selectModelKey]->IsSkinning() == true)
        {
            switch (renderType)
            {
            case RendererType::ONLYSPECULAR:
            {
                SkinnedMeshRenderComponent* rendercomp = new SkinnedMeshRenderComponent();
                rendercomp->Initalize(AssetClass::mMeshMap[selectModelKey], mShader, mSkinnedDepthShader, AssetClass::mTextureMap[selectTextureKey]);
                newGameObejct->InsertComponent(rendercomp);
            }
                break;
            case RendererType::BUMP:
            {
                SkinnedMeshBumpRenderComponent* bumpcomp = new SkinnedMeshBumpRenderComponent();
                bumpcomp->Initalize(AssetClass::mMeshMap[selectModelKey], mSkinnedBumpShader, mSkinnedDepthShader, AssetClass::mTextureMap[selectTextureKey], AssetClass::mTextureMap[selectBumpTextureKey], AssetClass::mTextureMap[selectSpecularTextureKey]);
                newGameObejct->InsertComponent(bumpcomp);
            }
                break;
            }


            AnimatorComponent* animComp = new AnimatorComponent();
            animComp->SetAnimation(anim);

            newGameObejct->InsertComponent(animComp);

            mGameObejcts[selectModelKey] = newGameObejct;
        }
        else
        {
            MeshRenderComponent* renderComp = new MeshRenderComponent();
            renderComp->Initalize(AssetClass::mMeshMap[selectModelKey], mSpecularShader, mDepthShader, AssetClass::mTextureMap[selectTextureKey]);

            newGameObejct->InsertComponent(renderComp);

            mGameObejcts[selectModelKey] = newGameObejct;
        }

        addGameObject = false;
        selectModelKey = "";
        selectTextureKey = "";
    }

    bool inspectorActive = true;
    auto search = mGameObejcts.find(currentGameObject);
    if (search != mGameObejcts.end())
    {
        ImGui::Begin("Inspector",&inspectorActive, ImGuiWindowFlags_None);

        TransformComponent* transformComp = search->second->GetComponent<TransformComponent>();

        if (transformComp != nullptr)
        {
            if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed))
            {
                XMFLOAT3 pos; 
                XMStoreFloat3(&pos,transformComp->GetPosition());
                float position[3] = {pos.x,pos.y,pos.z};
                XMFLOAT3 rot = transformComp->GetRotation();
                float rotation[3] = { rot.x,rot.y ,rot.z };
                XMFLOAT3 s = transformComp->GetScale();
                float scale[3] = { s.x,s.y,s.z };

                ImGui::InputFloat3("Position", position);
                ImGui::InputFloat3("Rotation", rotation);
                ImGui::InputFloat3("Scale", scale);

                transformComp->SetPosition(XMFLOAT3(position[0], position[1], position[2]));
                transformComp->SetRotation(XMFLOAT3(rotation[0], rotation[1], rotation[2]));
                transformComp->SetScale(XMFLOAT3(scale[0], scale[1], scale[2]));
            }
        }

        AnimatorComponent* animatorComp = search->second->GetComponent<AnimatorComponent>();
        if (animatorComp != nullptr)
        {
            if (ImGui::CollapsingHeader("Animator", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed))
            {
                if (ImGui::Button("Play"))
                {
                    animatorComp->Play();
                }
                if (ImGui::Button("Stop"))
                {
                    animatorComp->Stop();
                }
            }
        }

        ImGui::End();
    }

    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

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

    /*MeshRenderComponent* currentRenderer;
    currentRenderer = mGameObject->GetComponent<MeshRenderComponent>();

    if (currentRenderer != nullptr)
    {
        currentRenderer->RenderDepth(mDirect->GetDeviceContext());
    }*/

    for (const auto& gameObject : mGameObejcts)
    {
        MeshRenderComponent* renderComp = gameObject.second->GetComponent<MeshRenderComponent>();
        renderComp->RenderDepth(mDirect->GetDeviceContext());
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

    if (mCurrentPositionGizumoState != PositionGizumoState::NONE)
        return;


    if (mForwardArrowModel->RayIntersect(rayOrigin, rayDirection, position, XMFLOAT3(20.0f, 20.0f, 100.0f)))
    {
        mCurrentPositionGizumoState = PositionGizumoState::FORWARD;
    }
    else if (mRightArrowModel->RayIntersect(rayOrigin, rayDirection, position, XMFLOAT3(100.0f, 20.0f, 20.0f)))
    {
        mCurrentPositionGizumoState = PositionGizumoState::RIGHT;
    }
    else if (mUpArrowModel->RayIntersect(rayOrigin, rayDirection, position, XMFLOAT3(20.0f, 100.0f, 20.0f)))
    {
        mCurrentPositionGizumoState = PositionGizumoState::UP;
    }
}
