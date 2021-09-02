#include "GraphicsClass.h"
#include"SystemClass.h"
#include "Component.h"
#include "TransformComponent.h"
#include "MeshRenderComponent.h"
#include "SkinnedMeshRenderComponent.h"
#include "SkinnedMeshBumpRenderComponent.h"
#include "AnimatorComponent.h"
#include "GameObjectBrowser.h"
#include <typeinfo>
#include "Physics.h"

extern SystemClass* ApplicationHandle;
extern Camera* gMainCamera;
extern Light* gMainLight;
extern D3DClass* gDirect;
extern SimpleColorShader* gSimpleColorShader;

bool VectorItemGetter(void* list, int count, const char** outText)
{
    std::vector<std::string>& vector = *static_cast<std::vector<std::string>*>(list);
    if (count < 0 || count > vector.size())
    {
        return false;
    }
    *outText = vector[count].c_str();

    return true;
}

GraphicsClass::GraphicsClass() :
    mDirect(nullptr),
    mFbxLoader(nullptr),
    mLight(nullptr),
    mCamera(nullptr),
    mGrid(nullptr),
    mRenderTexture(nullptr),
    mSkinnedShader(nullptr),
    mSpecularShader(nullptr),
    mColorShader(nullptr),
    mDepthShader(nullptr),
    mShadowShader(nullptr),
    mSkinnedDepthShader(nullptr),
    mSolidShader(nullptr),
    mTextureShader(nullptr),
    mSkinnedBumpShader(nullptr),
    mSimpleColorShader(nullptr),
    mMouseBitmap(nullptr),
    mImgui(nullptr),
    gameObjectBrowser(nullptr),
    mForwardArrowModel(nullptr),
    mRightArrowModel(nullptr),
    mUpArrowModel(nullptr),
    mCurrentGameObject(""),
    mScreenWidth(0),
    mScreenHeight(0)
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

    mSkinnedShader = new SkinnedMeshShader;
    result = mSkinnedShader->Initialize(mDirect->GetDevice(),hwnd);
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

    mSimpleColorShader = new SimpleColorShader;
    result = mSimpleColorShader->Initialize(mDirect->GetDevice(), hwnd);
    if (result == false)
        return false;
    gSimpleColorShader = mSimpleColorShader;

    gameObjectBrowser = new GameObjectBrowser;

    mGameObjectNames.push_back("None");

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

    if (mCamera)
    {
        delete mCamera;
        mCamera = nullptr;
    }

    if (mSkinnedShader)
    {
        mSkinnedShader->Shutdown();
        delete mSkinnedShader;
        mSkinnedShader = nullptr;
    }

    if (mSpecularShader)
    {
        mSpecularShader->Shutdown();
        delete mSpecularShader;
        mSpecularShader = nullptr;
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

    if (mSimpleColorShader)
    {
        mSimpleColorShader->Shutdown();
        delete mSimpleColorShader;
        mSimpleColorShader = nullptr;
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

        mCamera->GetTransform().Rotate(InputClass::GetInstance()->GetMouseX() * ApplicationHandle->DeltaTime(), Axis::Yaxis);
        mCamera->GetTransform().Rotate(InputClass::GetInstance()->GetMouseY() * ApplicationHandle->DeltaTime(), Axis::Xaxis);
    }

    XMMATRIX worldMatrix, viewMarix, projectionMatrix;
    mDirect->GetWorldMatrix(worldMatrix);
    mDirect->GetProjectionMatrix(projectionMatrix);
    mCamera->GetViewMatrix(viewMarix);

    for (const auto& gameObject : mGameObejcts)
    {
        gameObject.second->Update(ApplicationHandle->DeltaTime());
    }

    for (const auto& gameObject : mGameObejcts)
    {
        gameObject.second->LateUpdate(ApplicationHandle->DeltaTime());
    }

    if (InputClass::GetInstance()->IsMouse0Down())
    {
        InputClass::GetInstance()->GetWMMouseLocation(mouseX, mouseY);

        if (mCurrentGameObject != "")
        {
            TransformComponent* transform = mGameObejcts[mCurrentGameObject]->GetComponent<TransformComponent>();
            XMFLOAT3 position;
            XMStoreFloat3(&position, transform->GetPosition());
            XMMATRIX positionMatrix = XMMatrixTranslation(position.x, position.y, position.z);
            if (Physics::TestIntersectionBoundBox(mouseX, mouseY, mScreenWidth, mScreenHeight, mCamera->GetPosition(), worldMatrix, viewMarix, projectionMatrix, positionMatrix, mForwardArrowModel->GetBounds()))
                mCurrentPositionGizumoState = PositionGizumoState::FORWARD;
            else if(Physics::TestIntersectionBoundBox(mouseX, mouseY, mScreenWidth, mScreenHeight, mCamera->GetPosition(), worldMatrix, viewMarix, projectionMatrix, positionMatrix, mRightArrowModel->GetBounds()))
                mCurrentPositionGizumoState = PositionGizumoState::RIGHT;
            else if (Physics::TestIntersectionBoundBox(mouseX, mouseY, mScreenWidth, mScreenHeight, mCamera->GetPosition(), worldMatrix, viewMarix, projectionMatrix, positionMatrix, mUpArrowModel->GetBounds()))
                mCurrentPositionGizumoState = PositionGizumoState::UP;
        }

        if (mCurrentPositionGizumoState == PositionGizumoState::NONE)
        {
            for (const auto& gameObject : mGameObejcts)
            {
                MeshRenderComponent* renderComp = gameObject.second->GetComponent<MeshRenderComponent>();
                if (renderComp != nullptr)
                {
                    if (Physics::TestIntersectionBoundBox(mouseX, mouseY, mScreenWidth, mScreenHeight, mCamera->GetPosition(), worldMatrix, viewMarix, projectionMatrix, gameObject.second->GetComponent<TransformComponent>()->GetTransform(), renderComp->GetMesh()->GetBounds()))
                        mCurrentGameObject = gameObject.first;
                }
            }
        }
    }
    if (InputClass::GetInstance()->IsMouse0Pressed() == false)
    {
        mCurrentPositionGizumoState = PositionGizumoState::NONE;
    }

    XMVECTOR cameraForward;
    XMVECTOR arrowDirection;
    float yWeight = 0.0f;
    float xWeight = 0.0f;

    if (mCurrentGameObject != "")
    {
        TransformComponent* transform = mGameObejcts[mCurrentGameObject]->GetComponent<TransformComponent>();
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
            transform->Translate(XMFLOAT3(0.f, 0.f, 1.f * deltaWeight));
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
            transform->Translate(XMFLOAT3(-1.f * deltaWeight, 0.f, 0.f));

        }
        break;
        case PositionGizumoState::UP:
        {
            cameraForward = XMLoadFloat3(&mCamera->GetTransform().Forward());
            arrowDirection = XMVectorSet(0.f, 1.f, 0.f, 0.f);
            float deltaWeight = -(ApplicationHandle->Input().GetMouseY());
            transform->Translate(XMFLOAT3(0.f, 1.f * deltaWeight, 0.f));
        }
        break;
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

    //°ÔÀÓ ¿ÀºêÁ§Æ® ·»´õ¸µ
    for (const auto& gameObject : mGameObejcts)
    {
        MeshRenderComponent* renderComp = gameObject.second->GetComponent<MeshRenderComponent>();
        renderComp->Render(mDirect->GetDeviceContext());
    }

    auto gameobject = mGameObejcts.find(mCurrentGameObject);
    if (gameobject != mGameObejcts.end())
    {
        TransformComponent* transform = mGameObejcts[mCurrentGameObject]->GetComponent<TransformComponent>();
        XMFLOAT3 position;
        XMStoreFloat3(&position, transform->GetPosition());
        XMMATRIX positionMat = XMMatrixTranslation(position.x, position.y, position.z);

        mForwardArrowModel->Render(mDirect->GetDeviceContext());
        mSolidShader->Render(mDirect->GetDeviceContext(), mForwardArrowModel->GetIndexCount(), positionMat, viewMatrix, projectionMatrix, XMFLOAT4(0.0f, 0.0f, 1.f, 1.f));

        mRightArrowModel->Render(mDirect->GetDeviceContext());
        mSolidShader->Render(mDirect->GetDeviceContext(), mRightArrowModel->GetIndexCount(), positionMat, viewMatrix, projectionMatrix, XMFLOAT4(1.0f, 0.0f, 0.f, 1.f));

        mUpArrowModel->Render(mDirect->GetDeviceContext());
        mSolidShader->Render(mDirect->GetDeviceContext(), mUpArrowModel->GetIndexCount(), positionMat, viewMatrix, projectionMatrix, XMFLOAT4(0.0f, 1.f, 0.f, 1.f));
    }

    //2D ·»´õ¸µ
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
    //2D ·»´õ¸µ ³¡

    bool loadFbx = false;
    bool loadAnim = false;
    fs::path loadPath;

    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    //IMGUI ·»´õ¸µ
    //mImgui->Render(&loadFbx,loadPath, meshMap[mCurrentRenderMesh].Transfrom(),meshMap,mCurrentRenderMesh,*mCamera,mLight,mCurrentRenderMesh == "" ? nullptr : &(meshMap[mCurrentRenderMesh]), wmX, wmY, &loadAnim, loadPath);
    static bool activeModelBrowser = false;
    static bool activeAnimBrowser = false;

    modelListBrowser.RenderLoadFileWindow(&activeModelBrowser, &activeAnimBrowser);

    modelListBrowser.RenderModelFileBrowser(&loadFbx, loadPath, &activeModelBrowser);
    modelListBrowser.RenderModelAnimationBrowser(&loadAnim, loadPath, &activeAnimBrowser);

    modelListBrowser.RenderCameraWindow(*mCamera);
    modelListBrowser.RenderLightState(*mLight);

    if (loadFbx == true)
    {
        mFbxLoader->LoadFbxFile(loadPath);
    }
    
    if (loadAnim == true)
    {
        mFbxLoader->LoadAnimation(loadPath);
    }

    static std::vector<std::string> gameObejctNames;
    gameObejctNames.clear();
    for (const auto& go : mGameObejcts)
    {
        gameObejctNames.push_back(go.first);
    }

    static std::string currentGameObject;
    static std::string prevGameObject = "";
    static bool addGameObject = false;
    static std::string selectModelKey;
    static std::string selectTextureKey;
    static std::string selectBumpTextureKey;
    static std::string selectSpecularTextureKey;
    static RendererType renderType = RendererType::ONLYSPECULAR;

    mPrevGameObject = mCurrentGameObject;
    modelListBrowser.RenderGameObjectList(&addGameObject,&mCurrentGameObject,&selectModelKey,&selectTextureKey, selectBumpTextureKey,selectSpecularTextureKey, gameObejctNames, renderType);

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
                rendercomp->Initalize(AssetClass::mMeshMap[selectModelKey], mSkinnedShader, mSkinnedDepthShader);
                newGameObejct->InsertComponent(rendercomp);
            }
                break;
            case RendererType::BUMP:
            {
                SkinnedMeshBumpRenderComponent* bumpcomp = new SkinnedMeshBumpRenderComponent();
                bumpcomp->Initalize(AssetClass::mMeshMap[selectModelKey], mSkinnedBumpShader, mSkinnedDepthShader);
                newGameObejct->InsertComponent(bumpcomp);
            }
                break;
            }

            AnimatorComponent* animComp = new AnimatorComponent();
            newGameObejct->InsertComponent(animComp);
            mGameObejcts[selectModelKey] = newGameObejct;
            mGameObjectNames.push_back(selectModelKey);
        }
        else
        {
            MeshRenderComponent* renderComp = new MeshRenderComponent();
            renderComp->Initalize(AssetClass::mMeshMap[selectModelKey], mSpecularShader, mDepthShader);
            newGameObejct->InsertComponent(renderComp);
            mGameObejcts[selectModelKey] = newGameObejct;
            mGameObjectNames.push_back(selectModelKey);
        }

        addGameObject = false;
        selectModelKey = "";
        selectTextureKey = "";
    }


    static vector<string> currentSelectAbleAnimation;
    static int currentAnimIndex = 0;

    bool inspectorActive = true;
    auto search = mGameObejcts.find(mCurrentGameObject);
    if (search != mGameObejcts.end())
    {
        MeshRenderComponent* renderComp = search->second->GetComponent<MeshRenderComponent>();
        if (mCurrentGameObject != mPrevGameObject && AssetClass::GetAnimationCount() != 0 && renderComp->GetMesh()->IsSkinning())
        {
            int boneCount = renderComp->GetMesh()->GetBones().size();

            currentSelectAbleAnimation.clear();
            currentSelectAbleAnimation.push_back("None");
            int idx = 0;
            currentAnimIndex = 0;
            const AnimationData* currentSetAnim = search->second->GetComponent<AnimatorComponent>()->GetAnimation();
            for (const auto& anim : AssetClass::mAnimationMap)
            {
                if (anim.second->keyFrames.size() == boneCount)
                {
                    idx++;
                    currentSelectAbleAnimation.push_back(anim.first);
                    if (currentSetAnim == anim.second)
                        currentAnimIndex = idx;
                }
            }
        }

        ImGui::Begin("Inspector",&inspectorActive, ImGuiWindowFlags_None);

        TransformComponent* transformComp = search->second->GetComponent<TransformComponent>();

        if (transformComp != nullptr)
        {
            if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed))
            {
                XMFLOAT3 pos = transformComp->GetLocalPosition();
                //XMStoreFloat3(&pos,transformComp->GetLocalPosition());
                float position[3] = {pos.x,pos.y,pos.z};
                XMFLOAT3 rot = transformComp->GetLocalRotation();
                float rotation[3] = { rot.x,rot.y ,rot.z };
                XMFLOAT3 s = transformComp->GetLocalScale();
                float scale[3] = { s.x,s.y,s.z };

                ImGui::InputFloat3("Position", position);
                ImGui::InputFloat3("Rotation", rotation);
                ImGui::InputFloat3("Scale", scale);

                transformComp->SetLocalPosition(XMFLOAT3(position[0], position[1], position[2]));
                transformComp->SetLocalRotation(XMFLOAT3(rotation[0], rotation[1], rotation[2]));
                transformComp->SetLocalScale(XMFLOAT3(scale[0], scale[1], scale[2]));

                if (transformComp->GetParent() == nullptr)
                    selectParentObject = 0;

                if (ImGui::Combo("Parent", &selectParentObject, VectorItemGetter, static_cast<void*>(&mGameObjectNames), mGameObjectNames.size(), 16))
                {
                    if (selectParentObject != 0)
                    {
                        TransformComponent* parent = mGameObejcts[mGameObjectNames[selectParentObject]]->GetComponent<TransformComponent>();
                        transformComp->SetParent(parent);
                    }
                }
            }
        }

        static vector<string> textureNames;

        if (textureNames.size() != AssetClass::GetTextures().size())
        {
            for (const auto& texture : AssetClass::GetTextures())
            {
                textureNames.push_back(texture.first);
            }
        }


        if (search->second->GetComponent<MeshRenderComponent>() != nullptr && search->second->GetComponent<MeshRenderComponent>()->GetRenderType()==eRendererType::SkinnedMeshRenderer)
        {
            SkinnedMeshRenderComponent* comp = search->second->GetComponent<SkinnedMeshRenderComponent>();
            if (ImGui::CollapsingHeader("SkinnedMeshRender", ImGuiTreeNodeFlags_Framed))
            {
                auto& subObjectMats = comp->GetObjectMaterials();
                for (int i = 0; i< subObjectMats.size(); i++)
                {   
                    string meshGroupLabel = "MeshGroup";
                    string num = to_string(i);
                    meshGroupLabel += num;
                    if (ImGui::CollapsingHeader(meshGroupLabel.c_str(), ImGuiTreeNodeFlags_Framed))
                    {
                        for (int j = 0; j < subObjectMats[i].size(); j++)
                        {
                            string subMeshLabel = "Material";
                            string num = to_string(j);
                            subMeshLabel += num;
                            if (ImGui::CollapsingHeader(subMeshLabel.c_str(), ImGuiTreeNodeFlags_Framed))
                            {
                                int selectTextureCount = 0;
                                for (int textIdx = 0; textIdx < textureNames.size(); textIdx++)
                                {
                                    if (subObjectMats[i][j].GetTextureKey() == textureNames[textIdx])
                                    {
                                        selectTextureCount = textIdx;
                                        break;
                                    }
                                }
                                if (ImGui::Combo("Diffuse", &selectTextureCount, VectorItemGetter,static_cast<void*>(&textureNames), textureNames.size(), 16))
                                {
                                    comp->SetMaterial(i,j, textureNames[selectTextureCount]);
                                }
                            }
                        }
                    }
                }
                bool visible = comp->IsVisibleBoundsBox();
                if (ImGui::Checkbox("Visible Bounds", &visible))
                    comp->SetVisibleBoundsBox(visible);
            }
        }
        else if (search->second->GetComponent<MeshRenderComponent>() != nullptr && search->second->GetComponent<MeshRenderComponent>()->GetRenderType() == eRendererType::MeshRenderer)
        {
            MeshRenderComponent* comp = search->second->GetComponent<MeshRenderComponent>();
            if (ImGui::CollapsingHeader("MeshRender", ImGuiTreeNodeFlags_Framed))
            {
                auto& subObjectMats = comp->GetObjectMaterials();
                for (int i = 0; i < subObjectMats.size(); i++)
                {
                    string meshGroupLabel = "MeshGroup";
                    string num = to_string(i);
                    meshGroupLabel += num;
                    if (ImGui::CollapsingHeader(meshGroupLabel.c_str(), ImGuiTreeNodeFlags_Framed))
                    {
                        for (int j = 0; j < subObjectMats[i].size(); j++)
                        {
                            string subMeshLabel = "Material";
                            string num = to_string(j);
                            subMeshLabel += num;
                            if (ImGui::CollapsingHeader(subMeshLabel.c_str(), ImGuiTreeNodeFlags_Framed))
                            {
                                int selectTextureCount = 0;
                                for (int textIdx = 0; textIdx < textureNames.size(); textIdx++)
                                {
                                    if (subObjectMats[i][j].GetTextureKey() == textureNames[textIdx])
                                    {
                                        selectTextureCount = textIdx;
                                        break;
                                    }
                                }
                                if (ImGui::Combo("Diffuse", &selectTextureCount, VectorItemGetter, static_cast<void*>(&textureNames), textureNames.size(), 16))
                                {
                                    comp->SetMaterial(i, j, textureNames[selectTextureCount]);
                                }
                            }
                        }
                    }
                }
                bool visible = comp->IsVisibleBoundsBox();
                if (ImGui::Checkbox("Visible Bounds", &visible))
                    comp->SetVisibleBoundsBox(visible);
            }
        }
        else if (search->second->GetComponent<MeshRenderComponent>() != nullptr && search->second->GetComponent<MeshRenderComponent>()->GetRenderType() == eRendererType::SkinnedBumpRenderer)
        {
            SkinnedMeshBumpRenderComponent* comp = search->second->GetComponent<SkinnedMeshBumpRenderComponent>();
            if (ImGui::CollapsingHeader("BumpSkinnedMeshRender",  ImGuiTreeNodeFlags_Framed))
            {
               auto& subObjectMats = comp->GetObjectMaterials();
                for (int i = 0; i < subObjectMats.size(); i++)
                {
                    string meshGroupLabel = "MeshGroup";
                    string meshGroupNum = to_string(i);
                    meshGroupLabel += meshGroupNum;
                    if (ImGui::CollapsingHeader(meshGroupLabel.c_str(), ImGuiTreeNodeFlags_Framed))
                    {
                        for (int j = 0; j < subObjectMats[i].size(); j++)
                        {
                            string subMeshLabel = "Material";
                            string num = to_string(j);
                            subMeshLabel += meshGroupNum +"_"+num;
                            if (ImGui::CollapsingHeader(subMeshLabel.c_str(), ImGuiTreeNodeFlags_Framed))
                            {
                                int selectTextureCount = 0;
                                int selectNormalCount = 0;
                                int selectSpecularCount = 0;
                                for (int textIdx = 0; textIdx < textureNames.size(); textIdx++)
                                {
                                    if (subObjectMats[i][j].GetTextureKey() == textureNames[textIdx])
                                    {
                                        selectTextureCount = textIdx;
                                    }
                                    if (subObjectMats[i][j].GetNormalKey() == textureNames[textIdx])
                                    {
                                        selectNormalCount = textIdx;
                                    }
                                    if (subObjectMats[i][j].GetSpecularKey() == textureNames[textIdx])
                                    {
                                        selectSpecularCount = textIdx;
                                    }
                                }
                                if (ImGui::Combo("Diffuse", &selectTextureCount, VectorItemGetter, static_cast<void*>(&textureNames), textureNames.size(), 16))
                                {
                                    comp->SetMaterial(i, j, textureNames[selectTextureCount], subObjectMats[i][j].GetNormalKey(), subObjectMats[i][j].GetSpecularKey());
                                }
                                if (ImGui::Combo("Normal", &selectNormalCount, VectorItemGetter, static_cast<void*>(&textureNames), textureNames.size(), 16))
                                {
                                    comp->SetMaterial(i, j, subObjectMats[i][j].GetTextureKey(), textureNames[selectNormalCount], subObjectMats[i][j].GetSpecularKey());
                                }
                                if (ImGui::Combo("Specular", &selectSpecularCount, VectorItemGetter, static_cast<void*>(&textureNames), textureNames.size(), 16))
                                {
                                    comp->SetMaterial(i, j, subObjectMats[i][j].GetTextureKey(), subObjectMats[i][j].GetNormalKey(), textureNames[selectSpecularCount]);
                                }
                            }
                        }
                    }
                }
                bool visible = comp->IsVisibleBoundsBox();
                if (ImGui::Checkbox("Visible Bounds", &visible))
                    comp->SetVisibleBoundsBox(visible);
            }
        }
        

        AnimatorComponent* animatorComp = search->second->GetComponent<AnimatorComponent>();
        if (animatorComp != nullptr)
        {
            if (ImGui::CollapsingHeader("Animator", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed))
            {
                if (currentSelectAbleAnimation.size() > 0)
                {
                    if (ImGui::Combo("SetAnimation", &currentAnimIndex, VectorItemGetter, static_cast<void*>(&currentSelectAbleAnimation), currentSelectAbleAnimation.size(), 16))
                    {
                        if (currentAnimIndex == 0)
                            animatorComp->SetAnimation(AssetClass::GetAnimation(nullptr));
                        else
                            animatorComp->SetAnimation(AssetClass::GetAnimation(currentSelectAbleAnimation[currentAnimIndex]));
                    }
                }

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

    for (const auto& gameObject : mGameObejcts)
    {
        MeshRenderComponent* renderComp = gameObject.second->GetComponent<MeshRenderComponent>();
        renderComp->RenderDepth(mDirect->GetDeviceContext());
    }


    mDirect->GetWorldMatrix(worldMatrix); 

    mDirect->SetBackBufferRenderTarget();

    mDirect->ResetViewport();

    return true;
}

