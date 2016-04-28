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
	m_sphereMesh = new SphereMesh(m_Direct3D->GetDevice(), L"../res/lava.png", 20);

	// Initialising the shaders.
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

	if (m_sphereMesh)
	{
		delete m_sphereMesh;
		m_sphereMesh = nullptr;
	}

	if (m_perlinNoiseShader)
	{
		delete m_perlinNoiseShader;
		m_perlinNoiseShader = nullptr;
	}

}

void ProceduralScene::RenderTheSphere(XMMATRIX& worldMatrix, XMMATRIX& viewMatrix, XMMATRIX& projectionMatrix)
{

	// The new translation, where we want our object to be.
	XMMATRIX new_transformation = XMMatrixTranslation(25.0f, 5.0f, -75.0f);
	XMMATRIX new_rotation = XMMatrixRotationRollPitchYaw(0.0f, sphereRotation, 0.0f);
	XMMATRIX new_scale = XMMatrixScaling(5.0f, 5.0f, 5.0f);

	// Multiplying the transformations together.
	worldMatrix = new_scale;
	worldMatrix *= new_rotation;
	worldMatrix *= new_transformation;

	// Render the sphere.
	m_sphereMesh->SendData(m_Direct3D->GetDeviceContext());
	m_perlinNoiseShader->SetShaderParameters(m_Direct3D->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, m_sphereMesh->GetTexture(), m_Timer);
	m_perlinNoiseShader->Render(m_Direct3D->GetDeviceContext(), m_sphereMesh->GetIndexCount());

	// Reset the world matrix.
	m_Direct3D->GetWorldMatrix(worldMatrix);

}

void ProceduralScene::RenderTheScene(float dt, XMMATRIX& worldMatrix, XMMATRIX& viewMatrix, XMMATRIX& projectionMatrix)
{

	// Rotate the sphere.
	SetSphereRotation(0.001f);

	// Render our procedurally manipulated sphere.
	RenderTheSphere(worldMatrix, viewMatrix, projectionMatrix);

}