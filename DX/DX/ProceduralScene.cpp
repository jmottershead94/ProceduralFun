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
	m_tree = new Model(m_Direct3D->GetDevice(), L"../res/newTreeTexture.dds", L"../res/Models/new_tree.obj");
	m_shrub = new Model(m_Direct3D->GetDevice(), L"../res/bushTexture.png", L"../res/Models/bush.obj");
	m_grass = new Model(m_Direct3D->GetDevice(), L"../res/grassTexture.dds", L"../res/Models/bush.obj");
	
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
	srand(time(0));

	for (int i = 0; i < 2; i++)
	{
		// Initialise the procedurally generated flora.
		InitialiseFlora(XMFLOAT3(0.0f, 0.0f, i * -12.5f));
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

void ProceduralScene::InitialiseFlora(XMFLOAT3 newStartPosition)
{

	// If we are already over the max flora limit.
	if (m_floraModels.size() > MAX_AMOUNT_OF_FLORA)
	{
		// Return from this function.
		return;
	}

	const int signChanger = -1;
	bool changeSign = false;
	XMFLOAT3 floraTranslation = { 0.0f, 0.0f, 0.0f };

	for (float i = 0.0f; i < 1.0f; (i += (1.0f / MAX_AMOUNT_OF_FLORA_PER_PATCH)))
	{
		// Randomly deciding on a sign change value for translations.
		int randomval = rand() % 2;
		changeSign = randomval;

		// Calculating the current flora ID number.
		noiseIDValue = m_simplexNoise->noise(i) * (ObjectIDNumber::ID_GRASS + 2);

		// Calculating the next flora translation value base on noise.
		floraTranslation = XMFLOAT3((m_simplexNoise->noise(i) * 50.0f) + newStartPosition.x, newStartPosition.y, (m_simplexNoise->noise(i)) + newStartPosition.z);

		// If we should change the sign for our values.
		if (changeSign)
		{
			// Reverse the z value so that we can create a more forest like feel.
			floraTranslation.z *= signChanger;

			// We no longer need to change the sign.
			changeSign = false;
		}


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
			// Adjusting the x translation value to make even spacing for all bushes.
			floraTranslation.x *= 2.0f;

			// Place in the current ID value.
			m_floraID.push_back(noiseIDValue);

			// Where this flora will be placed.
			m_floraTranslations.push_back(floraTranslation);

			// Place in a bush into the vector.
			m_floraModels.push_back(m_shrub);
		}
		else if (noiseIDValue == ObjectIDNumber::ID_GRASS)
		{
			// Place in the current ID value.
			m_floraID.push_back(noiseIDValue);

			// Where this flora will be placed.
			m_floraTranslations.push_back(floraTranslation);

			// Place in grass into the vector.
			m_floraModels.push_back(m_grass);
		}
		else
		{
			// Place in the current ID value.
			m_floraID.push_back(ObjectIDNumber::ID_GRASS);

			// Where this flora will be placed.
			m_floraTranslations.push_back(floraTranslation);

			// Place in grass into the vector.
			m_floraModels.push_back(m_grass);
		}
	}

}

void ProceduralScene::RemoveFlora()
{

	// Loop through the latest patch of flora.
	for (int i = 0; i < MAX_AMOUNT_OF_FLORA_PER_PATCH; i++)
	{
		// Assigning the current iterator index value for each of the flora vectors.
		std::vector<Model*>::iterator modelIndex = m_floraModels.begin() + i;
		std::vector<XMFLOAT3>::iterator translationIndex = m_floraTranslations.begin() + i;
		std::vector<int>::iterator idIndex = m_floraID.begin() + i;

		// If we have the more than the minimum amount of the flora specified.
		if (m_floraModels.size() > MAX_AMOUNT_OF_FLORA_PER_PATCH)
		{
			// Erase the current iterator index for this vector.
			m_floraModels.erase(modelIndex);
		}

		if (m_floraTranslations.size() > MAX_AMOUNT_OF_FLORA_PER_PATCH)
		{
			m_floraTranslations.erase(translationIndex);
		}

		if (m_floraID.size() > MAX_AMOUNT_OF_FLORA_PER_PATCH)
		{
			m_floraID.erase(idIndex);
		}
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

	// If we want to insert some more procedurally generated flora.
	if (m_Input->isKeyDown(VK_INSERT))
	{
		// Calculate a random value between 0 - 75.
		int randomval = rand() % 50;

		// Initialise a new patch of procedurally generated flora at randomly assigned positions.
		InitialiseFlora(XMFLOAT3(randomval, 0.0f, randomval * 0.5f));
	}
	else if (m_Input->isKeyDown(VK_DELETE))
	{
		// Remove the last patch of procedurally generated flora.
		RemoveFlora();
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
		// Otherwise, if we have bush model in our model vector.
		else if (m_floraID[i] == ObjectIDNumber::ID_GRASS)
		{
			// Render the grass model.
			RenderTheGrassModel(worldMatrix, viewMatrix, projectionMatrix, m_floraModels[i], m_floraTranslations[i]);
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
	XMMATRIX newRotation = XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f);
	XMMATRIX newScale = XMMatrixScaling(5.0f, 5.0f, 5.0f);

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
	//m_floraShader->SetShaderParameters(m_Direct3D->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, shrubModel->GetTexture(), m_perlinNoise->SimplexNoise(gravityDebug), ObjectIDNumber::ID_BUSH);
	m_floraShader->Render(m_Direct3D->GetDeviceContext(), shrubModel->GetIndexCount());

	// Reset the world matrix.
	m_Direct3D->GetWorldMatrix(worldMatrix);

}

void ProceduralScene::RenderTheGrassModel(XMMATRIX& worldMatrix, XMMATRIX& viewMatrix, XMMATRIX& projectionMatrix, Model* grassModel, XMFLOAT3 translation)
{

	// The new translation, where we want our object to be.
	XMMATRIX new_transformation = XMMatrixTranslation(translation.x, translation.y, translation.z);
	XMMATRIX new_rotation = XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f);
	XMMATRIX new_scale = XMMatrixScaling(0.0125f, 0.00675f, 0.0125f);
	//XMMATRIX new_scale = XMMatrixScaling(0.125f, 0.125f, 0.125f);

	// Multiplying the transformations together.
	worldMatrix = new_scale;
	worldMatrix *= new_rotation;
	worldMatrix *= new_transformation;

	// Render the tree.
	grassModel->SendData(m_Direct3D->GetDeviceContext());

	m_floraShader->SetShaderParameters(m_Direct3D->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, grassModel->GetTexture(), gravityDebug, ObjectIDNumber::ID_GRASS);
	m_floraShader->Render(m_Direct3D->GetDeviceContext(), grassModel->GetIndexCount());

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
	//RenderTheGrassModel(worldMatrix, viewMatrix, projectionMatrix, m_grass, XMFLOAT3(0.0f, 0.0f, -75.0f));

}