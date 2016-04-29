// Procedural scene.cpp
#include "ProceduralScene.h"

//////////////////////////////////////////////////////////
//======================================================//
//						Constructor						//
//======================================================//
// This will initialise all of our base scene pointers.	//
//////////////////////////////////////////////////////////
ProceduralScene::ProceduralScene(HWND hwnd, int screenWidth, int screenHeight, D3D* direct3D, Input *in, Camera* cam, Timer* timer) : BaseScene(hwnd, screenWidth, screenHeight, direct3D, in, cam, timer)
{

	// Initialising the meshes.
	m_proceduralFireSphereMesh = new SphereMesh(m_Direct3D->GetDevice(), L"../res/lava.png", 20);
	m_normalFireSphereMesh = new SphereMesh(m_Direct3D->GetDevice(), L"../res/lava.png", 20);
	m_proceduralWaterSphereMesh = new SphereMesh(m_Direct3D->GetDevice(), L"../res/cool_water.jpg", 20);
	m_normalWaterSphereMesh = new SphereMesh(m_Direct3D->GetDevice(), L"../res/cool_water.jpg", 20);
	m_proceduralLightningSphereMesh = new SphereMesh(m_Direct3D->GetDevice(), L"../res/lightning.jpg", 20);
	m_normalLightningSphereMesh = new SphereMesh(m_Direct3D->GetDevice(), L"../res/lightning.jpg", 20);

	// Initialising the shaders.
	m_textureShader = new TextureShader(m_Direct3D->GetDevice(), hwnd);
	m_perlinNoiseShader = new PerlinNoiseShader(m_Direct3D->GetDevice(), hwnd, ProceduralIDNumber::SUN);

	// Sphere rotation.
	sphereRotation = 0.0f;

}

//////////////////////////////////////////////////////////
//======================================================//
//						Destructor						//
//======================================================//
// This will release all of our pointers.				//
//////////////////////////////////////////////////////////
ProceduralScene::~ProceduralScene()
{

	if (m_proceduralFireSphereMesh)
	{
		delete m_proceduralFireSphereMesh;
		m_proceduralFireSphereMesh = nullptr;
	}

	if (m_normalFireSphereMesh)
	{
		delete m_normalFireSphereMesh;
		m_normalFireSphereMesh = nullptr;
	}

	if (m_proceduralWaterSphereMesh)
	{
		delete m_proceduralWaterSphereMesh;
		m_proceduralWaterSphereMesh = nullptr;
	}

	if (m_normalWaterSphereMesh)
	{
		delete m_normalWaterSphereMesh;
		m_normalWaterSphereMesh = nullptr;
	}

	if (m_proceduralLightningSphereMesh)
	{
		delete m_proceduralLightningSphereMesh;
		m_proceduralLightningSphereMesh = nullptr;
	}

	if (m_normalLightningSphereMesh)
	{
		delete m_normalLightningSphereMesh;
		m_normalLightningSphereMesh = nullptr;
	}

	if (m_textureShader)
	{
		delete m_textureShader;
		m_textureShader = nullptr;
	}

	if (m_perlinNoiseShader)
	{
		delete m_perlinNoiseShader;
		m_perlinNoiseShader = nullptr;
	}

}

void ProceduralScene::ProcessSphere(XMMATRIX& worldMatrix, XMMATRIX& viewMatrix, XMMATRIX& projectionMatrix, XMFLOAT3 position, SphereMesh* sphereMesh, bool isProcedural)
{

	// The new translation, where we want our object to be.
	XMMATRIX new_transformation = XMMatrixTranslation(position.x, position.y, position.z);
	XMMATRIX new_rotation = XMMatrixRotationRollPitchYaw(0.0f, sphereRotation, 0.0f);
	XMMATRIX new_scale = XMMatrixScaling(5.0f, 5.0f, 5.0f);

	// Multiplying the transformations together.
	worldMatrix = new_scale;
	worldMatrix *= new_rotation;
	worldMatrix *= new_transformation;

	// Render the sphere.
	sphereMesh->SendData(m_Direct3D->GetDeviceContext());

	// If the sphere uses procedural at all.
	if (isProcedural)
	{
		m_perlinNoiseShader->SetShaderParameters(m_Direct3D->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, sphereMesh->GetTexture(), m_Timer);
		m_perlinNoiseShader->Render(m_Direct3D->GetDeviceContext(), sphereMesh->GetIndexCount());
	}
	// Otherwise, we do not use procedural.
	else
	{
		m_textureShader->SetShaderParameters(m_Direct3D->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, sphereMesh->GetTexture());
		m_textureShader->Render(m_Direct3D->GetDeviceContext(), sphereMesh->GetIndexCount());
	}

	// Reset the world matrix.
	m_Direct3D->GetWorldMatrix(worldMatrix);

}

void ProceduralScene::RenderTheFireProceduralSphere(XMMATRIX& worldMatrix, XMMATRIX& viewMatrix, XMMATRIX& projectionMatrix)
{

	// This will process our specific sphere to the position that we want it in.
	ProcessSphere(worldMatrix, viewMatrix, projectionMatrix, XMFLOAT3(15.0f, 5.0f, -75.0f), m_proceduralFireSphereMesh, true);

}

void ProceduralScene::RenderTheFireSphere(XMMATRIX& worldMatrix, XMMATRIX& viewMatrix, XMMATRIX& projectionMatrix)
{

	// This will process our specific sphere to the position that we want it in.
	ProcessSphere(worldMatrix, viewMatrix, projectionMatrix, XMFLOAT3(30.0f, 5.0f, -75.0f), m_normalFireSphereMesh, false);

}

void ProceduralScene::RenderTheWaterProceduralSphere(XMMATRIX& worldMatrix, XMMATRIX& viewMatrix, XMMATRIX& projectionMatrix)
{

	// This will process our specific sphere to the position that we want it in.
	ProcessSphere(worldMatrix, viewMatrix, projectionMatrix, XMFLOAT3(45.0f, 5.0f, -75.0f), m_proceduralWaterSphereMesh, true);

}

void ProceduralScene::RenderTheWaterSphere(XMMATRIX& worldMatrix, XMMATRIX& viewMatrix, XMMATRIX& projectionMatrix)
{

	// This will process our specific sphere to the position that we want it in.
	ProcessSphere(worldMatrix, viewMatrix, projectionMatrix, XMFLOAT3(60.0f, 5.0f, -75.0f), m_normalWaterSphereMesh, false);

}

void ProceduralScene::RenderTheLightningProceduralSphere(XMMATRIX& worldMatrix, XMMATRIX& viewMatrix, XMMATRIX& projectionMatrix)
{

	// This will process our specific sphere to the position that we want it in.
	ProcessSphere(worldMatrix, viewMatrix, projectionMatrix, XMFLOAT3(75.0f, 5.0f, -75.0f), m_proceduralLightningSphereMesh, true);

}

void ProceduralScene::RenderTheLightningSphere(XMMATRIX& worldMatrix, XMMATRIX& viewMatrix, XMMATRIX& projectionMatrix)
{

	// This will process our specific sphere to the position that we want it in.
	ProcessSphere(worldMatrix, viewMatrix, projectionMatrix, XMFLOAT3(90.0f, 5.0f, -75.0f), m_normalLightningSphereMesh, false);

}

void ProceduralScene::RenderTheScene(float dt, XMMATRIX& worldMatrix, XMMATRIX& viewMatrix, XMMATRIX& projectionMatrix)
{

	// Rotate the sphere.
	SetSphereRotation(0.001f);

	// Render our procedurally manipulated sphere.
	RenderTheFireProceduralSphere(worldMatrix, viewMatrix, projectionMatrix);

	// Render the normal sphere.
	RenderTheFireSphere(worldMatrix, viewMatrix, projectionMatrix);

	// Render our procedurally manipulated sphere.
	RenderTheWaterProceduralSphere(worldMatrix, viewMatrix, projectionMatrix);

	// Render the normal sphere.
	RenderTheWaterSphere(worldMatrix, viewMatrix, projectionMatrix);

	// Render our procedurally manipulated sphere.
	RenderTheLightningProceduralSphere(worldMatrix, viewMatrix, projectionMatrix);

	// Render the normal sphere.
	RenderTheLightningSphere(worldMatrix, viewMatrix, projectionMatrix);

}