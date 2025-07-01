#include "DX3D/GameManager/GameManager.h"
#include "DX3D/Game/Display.h"
#include <DX3D/Graphics/RenderSystem.h>
#include <DX3D/Graphics/DeviceContext.h>
#include <DX3D/Graphics/SwapChain.h>
#include <DX3D/Graphics/VertexBuffer.h>
#include <DX3D/Math/GameMath.h>
#include <DX3D/Graphics/ResourceManager/MeshManager/Mesh.h>
#include <DX3D/Graphics/IndexBuffer.h>
#include <DX3D/Graphics/ConstantBuffer.h>
#include <DX3D/Input/InputSystem.h>
#include <DX3D/Graphics/GraphicsEngine.h>
#include <DX3D/Graphics/ResourceManager/TextureManager/TextureManager.h>
#include <DX3D/Object/GameObject.h>
#include <DX3D/Object/Transform.h>
#include <DX3D/Object/Axis.h>
#include <DX3D/Object/Material.h>
#include <DX3D/Physics/PhysicsEngine.h>
#include <DX3D/Physics/RigidBody.h>
#include <DX3D/Physics/Collider.h>
//gameObjects
#include <DX3D/Object/Objects/Camera.h>
#include <DX3D/Object/Objects/SkyBox.h>
#include <DX3D/Object/Objects/Player.h>
#include <string>
#include <DX3D/Object/Objects/Smoke.h>
#include <DX3D/Object/Objects/Grid.h>
dx3d::GameManager::GameManager(const BaseDesc& desc) : Base(desc)
{
	if (S == nullptr) {
		S = this;
	}
	else {
		DX3DLogErrorAndThrow("Game Manager has already been asigned!");
	}

	camera = std::make_shared<Camera>(BaseDesc{ m_logger });
	UINT resolution = 100;
	smokeHolder = std::make_shared<Smoke>(desc, resolution);
	smokeHolder->radius = 0.05f;
	smokeHolder->AddMaterial();
	std::shared_ptr<Texture> tex = GraphicsEngine::get()->getTextureManager().create2DSmokeTexture(resolution);
	smokeHolder->GetComponent<Material>()->textures.push_back(tex);
	smokeHolder->GetComponent<Material>()->SetPixelShader(L"DX3D/Shaders/Smoke/PixelShader.hlsl");
	smokeHolder->GetComponent<Material>()->SetVertexShader(L"DX3D/Shaders/Smoke/VertexShader.hlsl");
	smokeHolder->AddMesh(L"Quad");
	smokeHolder->GetComponent<Transform>()->rotation.SetRotationX(PI / 2.0f);
	smokeHolder->GetComponent<Transform>()->position.Translate(Vector3D(0, 0, 1));
	std::shared_ptr<Grid> grid = std::make_shared<Grid>();
	grid->LoadBorder();
	smokeHolder->AddComponent<Grid>(grid);
	gameObjects.push_back(smokeHolder);
}

void dx3d::GameManager::Update()
{
	InputSystem::get()->Update();
	PhysicsEngine::get()->Update();
	//camera->Update();
	CameraUpdate();
	for (int i = 0; i < gameObjects.size(); i++) {
		currentObject = gameObjects[i];
		currentObject->Update();
		GameObjectUpdate();
	}
}

void dx3d::GameManager::GameObjectUpdate()
{
	Display::get()->Draw(currentObject);
}

void dx3d::GameManager::CameraUpdate()
{
	Matrix4X4 lightRot;
	lightRot.SetIdentity();
	lightRotation += 0.25f * PI * Time::deltaTime;
	Matrix4X4 temp;
	temp.SetRotationX(-PI / 4.0f);
	lightRot *= temp;
	temp.SetRotationY(lightRotation);
	lightRot *= temp;
	Display::get()->CameraUpdate(lightRot, camera.get(), fov);
}

void dx3d::GameManager::LoadBasicScene()
{
	/*currentObject = GraphicsEngine::get()->createGameObject();
	currentObject->AddMesh(L"Cube");
	currentObject->AddCollider(ColliderType::Box);
	currentObject->AddRigidBody();
	currentObject->AddMaterial();
	currentObject->material->SetPixelShader(L"DX3D/Shaders/Lighting/SphereTex/PixelShader.hlsl");
	currentObject->material->SetVertexShader(L"DX3D/Shaders/Lighting/VertexShader.hlsl");
	currentObject->material->SetTexture(L"Assets/Textures/brick.png");
	currentObject->collider->show = true;
	currentObject->transform->scale.SetScale(Vector3D(1, 1, 1));
	currentObject->transform->position.SetTranslate(Vector3D(0, 10, 0));
	gameObjects.push_back(currentObject);*/


	currentObject = std::make_shared<Player>(BaseDesc{ m_logger });
	currentObject->AddMesh(L"Cube");
	currentObject->AddCollider(ColliderType::Box);
	currentObject->AddRigidBody();
	currentObject->AddMaterial();
	Material* material = currentObject->GetComponent<Material>();
	material->SetPixelShader(L"DX3D/Shaders/Lighting/SphereTex/PixelShader.hlsl");
	material->SetVertexShader(L"DX3D/Shaders/Lighting/VertexShader.hlsl");
	material->SetTexture(L"Assets/Textures/brick.png");
	Transform* transform = currentObject->GetComponent<Transform>();
	transform->scale.SetScale(Vector3D(1, 1, 1));
	transform->position.SetTranslate(Vector3D(0, 0, -4));
	gameObjects.push_back(currentObject);

	Transform* parent = transform;
	camera = std::make_shared<Camera>(BaseDesc{ m_logger });
	transform = camera->GetComponent<Transform>();
	transform->SetParent(parent);
	transform->position.SetTranslate(Vector3D(0, 1, -1));
	//gameObjects.push_back(std::static_pointer_cast<GameObject>(camera));

	currentObject = std::make_shared<SkyBox>(BaseDesc{ m_logger }, camera.get());
	currentObject->AddMeshFromFile(L"Assets/Meshes/sphere.obj");
	currentObject->AddMaterial();
	material = currentObject->GetComponent<Material>();
	material->SetTexture(L"Assets/Textures/sky_water.jpeg");
	material->SetPixelShader(L"DX3D/Shaders/Skybox/PixelShader.hlsl");
	material->SetVertexShader(L"DX3D/Shaders/Skybox/VertexShader.hlsl");
	material->rasterizerStateType = RasterizerStateType::BackFace;
	transform = currentObject->GetComponent<Transform>();
	transform->scale.SetScale(Vector3D(1, 1, 1) * 99);
	gameObjects.push_back(std::static_pointer_cast<GameObject>(currentObject));
	//Per Game Object
	//setting up mesh with the heart vertices and heart shaders

	currentObject = GraphicsEngine::get()->createGameObject();
	currentObject->AddMesh(L"Quad");
	currentObject->AddMaterial();
	currentObject->AddCollider(ColliderType::Quad);
	material = currentObject->GetComponent<Material>();
	material->SetPixelShader(L"DX3D/Shaders/Lighting/SphereTex/PixelShader.hlsl");
	material->SetVertexShader(L"DX3D/Shaders/Lighting/VertexShader.hlsl");
	material->SetTexture(L"Assets/Textures/grass.jpg");
	material->rasterizerStateType = RasterizerStateType::AllFace;
	transform = currentObject->GetComponent<Transform>();
	transform->scale.SetScale(Vector3D(10, 1, 10));
	transform->position.SetTranslate(Vector3D(0, -1, 0));
	gameObjects.push_back(currentObject);

	currentObject = GraphicsEngine::get()->createGameObject();
	currentObject->AddMesh(L"Cube");
	currentObject->AddCollider(ColliderType::Box);
	currentObject->AddRigidBody();
	currentObject->AddMaterial();
	material = currentObject->GetComponent <Material>();
	material->SetPixelShader(L"DX3D/Shaders/Lighting/SphereTex/PixelShader.hlsl");
	material->SetVertexShader(L"DX3D/Shaders/Lighting/VertexShader.hlsl");
	material->SetTexture(L"Assets/Textures/brick.png");
	transform = currentObject->GetComponent<Transform>();
	transform->scale.SetScale(Vector3D(1, 1, 1));
	transform->position.SetTranslate(Vector3D(0, 10, 0));
	gameObjects.push_back(currentObject);

	currentObject = GraphicsEngine::get()->createGameObject();
	currentObject->AddMeshFromFile(L"Assets/Meshes/sphere.obj");
	currentObject->AddMaterial();
	currentObject->AddCollider(ColliderType::Box);
	currentObject->AddRigidBody();
	material = currentObject->GetComponent<Material>();
	material->SetPixelShader(L"DX3D/Shaders/Lighting/SphereTex/PixelShader.hlsl");
	material->SetVertexShader(L"DX3D/Shaders/Lighting/VertexShader.hlsl");
	material->SetTexture(L"Assets/Textures/brick.png");
	transform = currentObject->GetComponent<Transform>();
	transform->scale.SetScale(Vector3D(1, 1, 1) * 1);
	transform->position.SetTranslate(Vector3D(3, 10, 0));
	gameObjects.push_back(currentObject);

	currentObject = GraphicsEngine::get()->createGameObject();
	currentObject->AddMeshFromFile(L"Assets/Meshes/statue.obj");
	currentObject->AddMaterial();
	currentObject->AddCollider(ColliderType::Box);
	Collider* collider = currentObject->GetComponent<Collider>();
	collider->SetTransform(Vector3D::up() * 1.2f + Vector3D::forward() * 0.2f);
	collider->SetScale(Vector3D(0.1f, 0.5f, 0.1f));
	currentObject->AddRigidBody();
	material = currentObject->GetComponent<Material>();
	material->SetPixelShader(L"DX3D/Shaders/Lighting/PixelShader.hlsl");
	material->SetVertexShader(L"DX3D/Shaders/Lighting/VertexShader.hlsl");
	transform = currentObject->GetComponent<Transform>();
	transform->scale.SetScale(Vector3D(1, 1, 1) * 5);
	transform->position.SetTranslate(Vector3D(-3, 10, 0));
	std::shared_ptr<Axis> axis = GraphicsEngine::get()->createAxis(AxisDesc{ 1 });
	currentObject->AddComponent<Axis>(axis);
	gameObjects.push_back(currentObject);

	currentObject = GraphicsEngine::get()->createGameObject();
	currentObject->AddMesh(L"Wave");
	Mesh* mesh = currentObject->GetComponent<Mesh>();
	mesh->Demo();
	currentObject->AddMaterial();
	material = currentObject->GetComponent<Material>();
	material->SetPixelShader(L"DX3D/Shaders/Wave/PixelShader.hlsl");
	material->SetVertexShader(L"DX3D/Shaders/Wave/VertexShader.hlsl");
	material->SetTexture(L"Assets/Textures/wave.jpg");
	material->rasterizerStateType = RasterizerStateType::AllFace;
	transform = currentObject->GetComponent<Transform>();
	transform->scale.SetScale(Vector3D(0.2f, 1, 0.2f));
	transform->position.SetTranslate(Vector3D(-10, -2, -10));
	gameObjects.push_back(currentObject);
}

void dx3d::GameManager::SmokeTest2DUpdate()
{
}
