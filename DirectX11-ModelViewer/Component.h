#pragma once
#include<unordered_map>
#include <directxmath.h>
#include"MeshClass.h"
#include"SpecularShaderClass.h"
using namespace DirectX;

struct null_t {};

template <class _component_t>
struct is_unique_component
{
	typedef typename _component_t::component_identifier_t	identifier_t;
	typedef typename identifier_t::parent_component_t		parent_t;

	enum
	{
		value = std::conditional<identifier_t::is_unique, std::true_type, is_unique_component<parent_t>>::type::value
	};
};
template <>
struct is_unique_component<null_t>
{
	enum { value = false };
};

template <class _component_t>
struct get_unique_component
{
	typedef typename _component_t::component_identifier_t	identifier_t;
	typedef typename identifier_t::parent_component_t		parent_t;

	typedef typename std::conditional<identifier_t::is_unique, _component_t, typename get_unique_component<parent_t>::type>::type type;
};
template <>
struct get_unique_component<null_t>
{
	typedef null_t type;
};


template <class _component_t>
struct get_component
{
	typedef typename std::conditional<
		is_unique_component<_component_t>::value,
		typename get_unique_component<_component_t>::type,
		_component_t>::type type;
};

template <class _component_t, class _parent_component_t, bool _is_unique>
struct _component_identifier
{
	typedef _component_t current_component_t;
	typedef _parent_component_t parent_component_t;

	enum { is_unique = _is_unique };
};

#define DEFINE_COMPONENT(component_name, parent_component_name, unique_component) \
	public: \
		typedef _component_identifier<component_name, parent_component_name, unique_component> component_identifier_t; \
		static size_t GetComponentId(void) { return reinterpret_cast<size_t>(&identifier); } \
	private: \
		static component_identifier_t identifier; \

#define DECLARE_COMPONENT(component_name) \
	component_name::component_identifier_t component_name::identifier;

class GameObjectClass;
class BaseComponent
{
public:
	DEFINE_COMPONENT(BaseComponent, null_t, false);

public:
	virtual void Start() = 0;
	virtual void Update(float deltaTime) = 0;
	virtual void LateUpdate(float deltaTime) = 0;
	virtual void Destroy() = 0;

public:
	void SetOwnerGameObject(GameObjectClass* owner)
	{
		mOwnerGameObject = owner;
	}

protected:
	GameObjectClass* mOwnerGameObject;
};

DECLARE_COMPONENT(BaseComponent);

class GameObjectClass
{
public:
	static GameObjectClass* Create() { return new GameObjectClass; }

public:
	GameObjectClass() = default;
	~GameObjectClass() = default;

	void Start()
	{
		for (const auto& comp : components) { comp.second->Start(); }
	}

    void Update(float deltaTime)
	{
		for (const auto& comp : components) { comp.second->Update(deltaTime); }
	}
    void LateUpdate(float deltaTime)
	{
		for (const auto& comp : components) { comp.second->LateUpdate(deltaTime); }
	}
    void Destroy()
	{
		for (const auto& comp : components) { comp.second->Destroy(); }
	}

public:
	template<class component_t>
	void InsertComponent(component_t* comp)
	{
		size_t compId = get_component<component_t>::type::GetComponentId();
		this->components[compId] = comp;
		static_cast<BaseComponent*>(comp)->SetOwnerGameObject(this);
	}

	template<class component_t>
	component_t* GetComponent()
	{
		size_t compId = get_component<component_t>::type::GetComponentId();
		if (this->components.find(compId) == this->components.end())
			return nullptr;

		return reinterpret_cast<component_t*>(this->components[compId]);
	}
private:
	std::unordered_map<size_t, BaseComponent*> components;
};


//class TransformComponent : public BaseComponent
//{
//public:
//	DEFINE_COMPONENT(TransformComponent, BaseComponent, true);
//
//public:
//	TransformComponent() :
//		mPosition(0.f, 0.f, 0.f),
//		mRotation(0.f, 0.f, 0.f),
//		mScale(1.f, 1.f, 1.f)
//	{
//		UpdateTransform();
//	}
//	~TransformComponent() = delete;
//
//public:
//	virtual void Start() override {};
//	virtual void Update() override {};
//	virtual void LateUpdate() override {};
//	virtual void Destroy() override {};
//
//	XMVECTOR GetPosition() const { return XMLoadFloat3(&mPosition); }
//	XMFLOAT3 GetRotation() const { return mRotation; }
//	XMFLOAT3 GetScale() const { return mScale; }
//	XMMATRIX GetTransform() const { return mTransform; }
//
//	void SetPosition(XMFLOAT3 position)
//	{
//		mPosition = position;
//		UpdateTransform();
//	}
//
//	void SetRotation(XMFLOAT3 rotation)
//	{
//		mRotation = rotation;
//		UpdateTransform();
//	}
//
//	void SetScale(XMFLOAT3 scale)
//	{
//		mScale = scale;
//		UpdateTransform();
//	}
//
//	void UpdateTransform()
//	{
//		mTransform = XMMatrixScaling(mScale.x, mScale.y, mScale.z)
//			* XMMatrixRotationRollPitchYaw(XMConvertToRadians(mRotation.x), XMConvertToRadians(mRotation.y), XMConvertToRadians(mRotation.z)) * XMMatrixTranslation(mPosition.x, mPosition.y, mPosition.z);
//	}
//
//
//	XMVECTOR Forward() {return XMVector3Normalize(mTransform.r[3]);}
//	XMVECTOR Right() {	return XMVector3Normalize(mTransform.r[1]);}
//	XMVECTOR Up() {return XMVector3Normalize(mTransform.r[2]);}
//
//private:
//	XMFLOAT3 mPosition;
//	XMFLOAT3 mRotation;
//	XMFLOAT3 mScale;
//	XMMATRIX mTransform;
//};
//
//DECLARE_COMPONENT(TransformComponent);
//
//extern Camera* gMainCamera;
//extern Light* gMainLight;
//extern D3DClass* gDirect;
//
//class MeshRenderComponent : public BaseComponent
//{
//public:
//	DEFINE_COMPONENT(MeshRenderComponent, BaseComponent, true);
//
//public:
//	MeshRenderComponent() :
//		mMesh(nullptr),
//		mShader(nullptr),
//		mTexture(nullptr)
//	{}
//	~MeshRenderComponent() = default;
//
//public:
//	virtual void Start() override {};
//	virtual void Update() override {};
//	virtual void LateUpdate() override {};
//	virtual void Destroy() override {};
//
//	virtual void Render(ID3D11DeviceContext* deviceContext)
//	{
//		XMMATRIX viewMatrix, projectionMatrix;
//		gMainCamera->GetViewMatrix(viewMatrix);
//		gDirect->GetOrthoMatrix(projectionMatrix);
//
//		mMesh->Render(deviceContext);
//
//		TransformComponent* objectTransform = mOwnerGameObject->GetComponent<TransformComponent>();
//
//		mShader->Render(deviceContext, mMesh->GetIndexCount(), objectTransform->GetTransform(), viewMatrix, projectionMatrix, mTexture->GetTexture(), gMainLight->GetPosition(), gMainLight->GetDiffuseColor(), gMainLight->GetAmbientColor(), gMainCamera->GetPosition(), gMainLight->GetSpecularColor(), gMainLight->GetSpecularPower());
//	}
//
//public:
//	bool Initalize(MeshClass* mesh, SpecularShaderClass* shader, Texture* texture)
//	{
//		if (mesh == nullptr || shader == nullptr || texture == nullptr)
//			return false;
//
//		if (mesh->IsInitalized() == false)
//			return false;
//
//		mMesh = mesh;
//		mShader = shader;
//		mTexture = texture;
//
//		return true;
//	}
//
//protected:
//	MeshClass* mMesh;
//
//private:
//	SpecularShaderClass* mShader;
//	Texture* mTexture;
//};
//
//DECLARE_COMPONENT(MeshRenderComponent);
//
