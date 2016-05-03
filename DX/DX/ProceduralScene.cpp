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
	m_tree = new Model(m_Direct3D->GetDevice(), L"../res/tree_tex.png", L"../res/Models/tree.obj");
	m_shrub = new Model(m_Direct3D->GetDevice(), L"../res/grassTexture.png", L"../res/Models/bush.obj");
	//m_grass = new Model(m_Direct3D->GetDevice(), L"../res/grassTexture.png", L"../res/Models/grass.obj");

	// Initialising the shaders.
	m_textureShader = new TextureShader(m_Direct3D->GetDevice(), hwnd);
	m_perlinNoiseShader = new PulsingPlanetShader(m_Direct3D->GetDevice(), hwnd, ProceduralIDNumber::PLANET);
	m_floraShader = new FloraShader(m_Direct3D->GetDevice(), hwnd, ProceduralIDNumber::FLORA);

	// Sphere rotation.
	sphereRotation = 0.0f;
	treeRotation = {0.0f, 0.0f, 0.0f};
	gravityDebug = 1.0f;

	// Initialise the seed for our perlin noise selection.
	m_perlinNoise = new PerlinNoise();
	m_simplexNoise = new SimplexNoise(1.0f, 1.0f, 2.0f, 0.5f);
	
	XMFLOAT3 floraTranslation = {0.0f, 0.0f, 0.0f};

	for (float i = 0.0f; i < ((float)MAX_AMOUNT_OF_FLORA / (float)MAX_AMOUNT_OF_FLORA); (i += (1.0f / MAX_AMOUNT_OF_FLORA)))
	{
		noiseIDValue = m_simplexNoise->noise(i * 0.5f) * ObjectIDNumber::ID_GRASS;
		floraTranslation = XMFLOAT3(i * 100.0f, 0.0f, m_simplexNoise->noise(i) * -100.0f);

		if (noiseIDValue == ObjectIDNumber::ID_TREE)
		{
			// Place in the current ID value.
			m_floraID.push_back(noiseIDValue);

			// Where this flora will be placed.
			m_floraTranslations.push_back(floraTranslation);

			// Place in a tree into the vector.
			m_floraModels.push_back(m_tree);
		}
		else if (noiseIDValue == ObjectIDNumber::ID_BUSH)
		{
			// Place in the current ID value.
			m_floraID.push_back(noiseIDValue);

			// Where this flora will be placed.
			m_floraTranslations.push_back(floraTranslation);

			// Place in a bush into the vector.
			m_floraModels.push_back(m_shrub);
		}
		else if (noiseIDValue == ObjectIDNumber::ID_GRASS)
		{
			//m_floraModels.push_back();
		}
	}
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

	if (m_tree)
	{
		delete m_tree;
		m_tree = nullptr;
	}

	if (m_shrub)
	{
		delete m_shrub;
		m_shrub = nullptr;
	}

	if (m_grass)
	{
		delete m_grass;
		m_grass = nullptr;
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

	if (m_floraShader)
	{
		delete m_floraShader;
		m_floraShader = nullptr;
	}

	if (m_perlinNoise)
	{
		delete m_perlinNoise;
		m_perlinNoise = nullptr;
	}

}

void ProceduralScene::Controls(float dt)
{

	if (m_Input->isKeyDown('P'))
	{
		treeRotation.x += 0.001f;
	}
	else if (m_Input->isKeyDown('Y'))
	{
		treeRotation.y += 0.001f;
	}
	else if (m_Input->isKeyDown('R'))
	{
		treeRotation.z += 0.001f;
	}

	if (m_Input->isLeftMouseDown())
	{
		gravityDebug += 0.01f;
	}
	else if (m_Input->isRightMouseDown())
	{
		gravityDebug -= 0.01f;
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

void ProceduralScene::ProcessFlora(XMMATRIX& worldMatrix, XMMATRIX& viewMatrix, XMMATRIX& projectionMatrix)
{

	for (int i = 0; i < m_floraModels.size(); i++)
	{
		// If our tree model didn't have stupid axis alignment, this would be perfect.
		//RenderTheFloraModel(worldMatrix, viewMatrix, projectionMatrix, m_floraModels[i], m_floraTranslations[i], m_floraID[i]);

		// If we have a tree model in our model vector.
		if (m_floraID[i] == ObjectIDNumber::ID_TREE)
		{
			// Render the tree model.
			RenderTheTreeModel(worldMatrix, viewMatrix, projectionMatrix, m_floraModels[i], m_floraTranslations[i]);
		}
		// Otherwise, if we have bush model in our model vector.
		else if (m_floraID[i] == ObjectIDNumber::ID_BUSH)
		{
			// Render the bush model.
			RenderTheShrubModel(worldMatrix, viewMatrix, projectionMatrix, m_floraModels[i], m_floraTranslations[i]);
		}
	}

}

void ProceduralScene::RenderTheFloraModel(XMMATRIX& worldMatrix, XMMATRIX& viewMatrix, XMMATRIX& projectionMatrix, Model* floraModel, XMFLOAT3 translation, int IDNumber)
{

	// The new translation, where we want our object to be.
	XMMATRIX newTransformation = XMMatrixTranslation(translation.x, translation.y, translation.z);
	XMMATRIX newRotation = XMMatrixRotationRollPitchYaw(1.5f, 0.0f, 0.0f);
	XMMATRIX newScale = XMMatrixScaling(0.25f, 0.25f, 0.25f);

	// Multiplying the transformations together.
	worldMatrix = newScale;
	worldMatrix *= newRotation;
	worldMatrix *= newTransformation;

	// Render the tree.
	floraModel->SendData(m_Direct3D->GetDeviceContext());

	m_floraShader->SetShaderParameters(m_Direct3D->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, floraModel->GetTexture(), gravityDebug, IDNumber);
	//m_floraShader->SetShaderParameters(m_Direct3D->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, floraModel->GetTexture(), m_perlinNoise->SimplexNoise(gravityDebug), ObjectIDNumber::ID_TREE);
	m_floraShader->Render(m_Direct3D->GetDeviceContext(), floraModel->GetIndexCount());

	// Reset the world matrix.
	m_Direct3D->GetWorldMatrix(worldMatrix);

}

void ProceduralScene::RenderTheTreeModel(XMMATRIX& worldMatrix, XMMATRIX& viewMatrix, XMMATRIX& projectionMatrix, Model* treeModel, XMFLOAT3 translation)
{

	// The new translation, where we want our object to be.
	XMMATRIX newTransformation = XMMatrixTranslation(translation.x, translation.y, translation.z);
	XMMATRIX newRotation = XMMatrixRotationRollPitchYaw(1.5f, 0.0f, 0.0f);
	XMMATRIX newScale = XMMatrixScaling(0.25f, 0.25f, 0.25f);

	// Multiplying the transformations together.
	worldMatrix = newScale;
	worldMatrix *= newRotation;
	worldMatrix *= newTransformation;

	// Render the tree.
	treeModel->SendData(m_Direct3D->GetDeviceContext());

	m_floraShader->SetShaderParameters(m_Direct3D->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, treeModel->GetTexture(), gravityDebug, ObjectIDNumber::ID_TREE);
	//m_floraShader->SetShaderParameters(m_Direct3D->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, m_tree->GetTexture(), m_perlinNoise->SimplexNoise(gravityDebug), ObjectIDNumber::ID_TREE);
	m_floraShader->Render(m_Direct3D->GetDeviceContext(), treeModel->GetIndexCount());
	
	// Reset the world matrix.
	m_Direct3D->GetWorldMatrix(worldMatrix);

}

void ProceduralScene::RenderTheShrubModel(XMMATRIX& worldMatrix, XMMATRIX& viewMatrix, XMMATRIX& projectionMatrix, Model* shrubModel, XMFLOAT3 translation)
{

	// The new translation, where we want our object to be.
	XMMATRIX newTransformation = XMMatrixTranslation(translation.x, translation.y, translation.z);
	XMMATRIX newRotation = XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f);
	XMMATRIX newScale = XMMatrixScaling(0.0625f, 0.0625f, 0.0625f);

	// Multiplying the transformations together.
	worldMatrix = newScale;
	worldMatrix *= newRotation;
	worldMatrix *= newTransformation;

	// Render the bush.
	shrubModel->SendData(m_Direct3D->GetDeviceContext());

	m_floraShader->SetShaderParameters(m_Direct3D->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, shrubModel->GetTexture(), gravityDebug, ObjectIDNumber::ID_BUSH);
	//m_floraShader->SetShaderParameters(m_Direct3D->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, m_shrub->GetTexture(), m_perlinNoise->SimplexNoise(gravityDebug), ObjectIDNumber::ID_BUSH);
	m_floraShader->Render(m_Direct3D->GetDeviceContext(), shrubModel->GetIndexCount());

	// Reset the world matrix.
	m_Direct3D->GetWorldMatrix(worldMatrix);

}

void ProceduralScene::RenderTheGrassModel(XMMATRIX& worldMatrix, XMMATRIX& viewMatrix, XMMATRIX& projectionMatrix)
{

	// The new translation, where we want our object to be.
	XMMATRIX new_transformation = XMMatrixTranslation(35.0f, 0.0f, -60.0f);
	XMMATRIX new_rotation = XMMatrixRotationRollPitchYaw(0.0f, sphereRotation, 0.0f);
	XMMATRIX new_scale = XMMatrixScaling(1.0f, 1.0f, 1.0f);

	// Multiplying the transformations together.
	worldMatrix = new_scale;
	worldMatrix *= new_rotation;
	worldMatrix *= new_transformation;

	// Render the tree.
	m_grass->SendData(m_Direct3D->GetDeviceContext());

	m_floraShader->SetShaderParameters(m_Direct3D->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, m_grass->GetTexture(), 0.25f, ObjectIDNumber::ID_GRASS);
	m_floraShader->Render(m_Direct3D->GetDeviceContext(), m_grass->GetIndexCount());

	// Reset the world matrix.
	m_Direct3D->GetWorldMatrix(worldMatrix);

}

void ProceduralScene::RenderTheScene(float dt, XMMATRIX& worldMatrix, XMMATRIX& viewMatrix, XMMATRIX& projectionMatrix)
{

	// Rotate the spheres.
	sphereRotation += 0.001f;

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

	// Process all of the procedural flora.
	ProcessFlora(worldMatrix, viewMatrix, projectionMatrix);

	// Render the tree model.
	//RenderTheTreeModel(worldMatrix, viewMatrix, projectionMatrix, m_perlinNoise->Noise(2.00, 1.97, 9.84));

	// Render the tree model.
	//RenderTheTreeModel(worldMatrix, viewMatrix, projectionMatrix);

	// Render the shrub model.
	//RenderTheShrubModel(worldMatrix, viewMatrix, projectionMatrix);

	// Render the grass model.
	//RenderTheGrassModel(worldMatrix, viewMatrix, projectionMatrix);

}