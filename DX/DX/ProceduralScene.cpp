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
	m_planeMesh = new PlaneMesh(m_Direct3D->GetDevice(), L"../res/terrainDirt.jpg");
	m_renderTexture = new RenderTexture(m_Direct3D->GetDevice(), screenWidth, screenHeight, SCREEN_NEAR, SCREEN_DEPTH);
	m_renderBlurTexture = new RenderTexture(m_Direct3D->GetDevice(), screenWidth, screenHeight, SCREEN_NEAR, SCREEN_DEPTH);
	m_downSampleTexture = new RenderTexture(m_Direct3D->GetDevice(), screenWidth / 2, screenHeight / 2, SCREEN_NEAR, SCREEN_DEPTH);		//Downscale
	m_horizontalSampleTexture = new RenderTexture(m_Direct3D->GetDevice(), screenWidth / 2, screenHeight / 2, SCREEN_NEAR, SCREEN_DEPTH);		//Horizontal
	m_verticalSampleTexture = new RenderTexture(m_Direct3D->GetDevice(), screenWidth / 2, screenHeight / 2, SCREEN_NEAR, SCREEN_DEPTH);		//Vertical
	m_upSampleTexture = new RenderTexture(m_Direct3D->GetDevice(), screenWidth, screenHeight, SCREEN_NEAR, SCREEN_DEPTH);				//Upscale
	m_blurredMesh = new OrthoMesh(m_Direct3D->GetDevice(), m_screenWidth, m_screenHeight, 0, 0);

	// Initialising the shaders.
	m_textureShader = new TextureShader(m_Direct3D->GetDevice(), hwnd);
	m_perlinNoiseShader = new PulsingPlanetShader(m_Direct3D->GetDevice(), hwnd, ProceduralIDNumber::PLANET);
	m_floraShader = new FloraShader(m_Direct3D->GetDevice(), hwnd, ProceduralIDNumber::FLORA);
	m_horizontalBlurShader = new HorizontalBlurShader(m_Direct3D->GetDevice(), hwnd);
	m_verticalBlurShader = new VerticalBlurShader(m_Direct3D->GetDevice(), hwnd);

	// Sphere rotation.
	sphereRotation = 0.0f;

	// Flora variables.
	treeRotation = {0.0f, 0.0f, 0.0f};
	gravityControl = 1.0f;

	// Gaussian Blur flag.
	m_activateGaussianBlur = false;

	// Initialise the seed for our perlin noise selection.
	m_perlinNoise = new PerlinNoise();
	m_simplexNoise = new SimplexNoise(1.0f, 1.0f, 2.0f, 0.5f);
	srand(time(0));

	for (int i = 0; i < 2; i++)
	{
		// Initialise the procedurally generated flora.
		InitialiseFlora(XMFLOAT3(0.0f, 0.0f, i * -50.0f));
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

	if (m_simplexNoise)
	{
		delete m_simplexNoise;
		m_simplexNoise = nullptr;
	}

	if (!m_floraModels.empty())
	{
		m_floraModels.clear();
	}

	if (!m_floraTranslations.empty())
	{
		m_floraTranslations.clear();
	}

	if (!m_floraID.empty())
	{
		m_floraID.clear();
	}

	if (!m_floraGravity.empty())
	{
		m_floraGravity.clear();
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
	m_floraGravity.push_back(m_simplexNoise->noise(0.5f) * (rand() % 5));

	for (float i = 0.0f; i < 1.0f; (i += (1.0f / MAX_AMOUNT_OF_FLORA_PER_PATCH)))
	{
		// Randomly deciding on a sign change value for translations.
		int randomval = rand() % 2;
		changeSign = randomval;

		// Calculating the current flora ID number.
		noiseIDValue = m_simplexNoise->noise(i) * (ObjectIDNumber::ID_GRASS + 2);

		// Calculating the next flora translation value base on noise.
		floraTranslation = XMFLOAT3((m_simplexNoise->noise(i) * 25.0f) + newStartPosition.x, newStartPosition.y, (m_simplexNoise->noise(i)) + newStartPosition.z);

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
		else
		{
			// Adjusting the x translation value to make even spacing for all bushes.
			floraTranslation.x *= 2.0f;

			// Place in the current ID value.
			m_floraID.push_back(noiseIDValue);

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

	// Gravity will affect an entire patch of flora.
	// Should loop through the amount of gravity scales there are.
	for (int i = 0; i < m_floraGravity.size(); i++)
	{
		// If we have the more than the minimum amount of the flora specified.
		if (m_floraGravity.size() > 1)
		{
			std::vector<float>::iterator gravityIndex = m_floraGravity.begin() + i;

			// Erase the current iterator index for this vector.
			m_floraGravity.erase(gravityIndex);
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
		gravityControl += 0.01f;
	}
	else if (m_Input->isRightMouseDown())
	{
		gravityControl -= 0.01f;
	}

	// If we want to insert some more procedurally generated flora.
	if (m_Input->isKeyDown(VK_INSERT))
	{
		// Calculate a random value between 0 - 75.
		int randomval = rand() % 25;

		// Initialise a new patch of procedurally generated flora at randomly assigned positions.
		InitialiseFlora(XMFLOAT3(randomval, 0.0f, randomval * 0.5f));
	}
	else if (m_Input->isKeyDown(VK_DELETE))
	{
		// Remove the last patch of procedurally generated flora.
		RemoveFlora();
	}

	if (m_Input->isKeyDown('9'))
	{
		m_activateGaussianBlur = true;
	}
	else if (m_Input->isKeyDown('0'))
	{
		m_activateGaussianBlur = false;
	}

}

void ProceduralScene::RenderToTexture(float dt)
{

	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;

	// Set the render target to be the render to texture.
	m_renderTexture->SetRenderTarget(m_Direct3D->GetDeviceContext());

	// Clear the render to texture.
	m_renderTexture->ClearRenderTarget(m_Direct3D->GetDeviceContext(), 0.0f, 0.0f, 1.0f, 0.5f);

	// Generate the view martrix based on the camera's position.
	m_Camera->Update();

	// Get the world, view and projection matrices form the camera and d3d objects.
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);

	// Render everything within our scene.
	//RenderTheScene(dt, worldMatrix, viewMatrix, projectionMatrix);

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

	// Render the dirt ground.
	RenderTheGround(worldMatrix, viewMatrix, projectionMatrix);

	// Process all of the procedural flora.
	ProcessFlora(worldMatrix, viewMatrix, projectionMatrix);

	// Set the back buffer render target.
	m_Direct3D->SetBackBufferRenderTarget();

}

void ProceduralScene::DownSampleTexture()
{

	XMMATRIX worldMatrix, orthoMatrix, baseViewMatrix;

	m_downSampleTexture->SetRenderTarget(m_Direct3D->GetDeviceContext());								// Set the render target to be the render to texture.
	m_downSampleTexture->ClearRenderTarget(m_Direct3D->GetDeviceContext(), 0.0f, 0.0f, 1.0f, 1.0f);		// Clear the render to texture.

	m_Camera->Update();

	// Get the world matrix.
	m_Direct3D->GetWorldMatrix(worldMatrix);	

	// Turn off the z buffer and start work on the blur.
	m_Direct3D->TurnZBufferOff();
	m_Direct3D->GetOrthoMatrix(orthoMatrix);
	m_Camera->GetBaseViewMatrix(baseViewMatrix);

	// Render our blurred mesh.
	m_blurredMesh->SendData(m_Direct3D->GetDeviceContext());
	m_textureShader->SetShaderParameters(m_Direct3D->GetDeviceContext(), worldMatrix, baseViewMatrix, orthoMatrix, m_renderTexture->GetShaderResourceView());
	m_textureShader->Render(m_Direct3D->GetDeviceContext(), m_blurredMesh->GetIndexCount());
	
	// Turn the z buffer back on.
	m_Direct3D->TurnZBufferOn();

}

void ProceduralScene::RenderHorizontalBlurTexture()
{

	XMMATRIX worldMatrix, orthoMatrix, baseViewMatrix;

	m_horizontalSampleTexture->SetRenderTarget(m_Direct3D->GetDeviceContext());		// Set the render target to be the render to texture.
	m_horizontalSampleTexture->ClearRenderTarget(m_Direct3D->GetDeviceContext(), 0.0f, 0.0f, 1.0f, 1.0f);		// Clear the render to texture.

	m_Camera->Update();		// Generate the view matrix based on the camera's position.
	
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Direct3D->GetOrthoMatrix(orthoMatrix);
	m_Camera->GetBaseViewMatrix(baseViewMatrix);

	m_Direct3D->TurnZBufferOff();

	m_blurredMesh->SendData(m_Direct3D->GetDeviceContext());
	m_horizontalBlurShader->SetShaderParameters(m_Direct3D->GetDeviceContext(), worldMatrix, baseViewMatrix, orthoMatrix, m_downSampleTexture->GetShaderResourceView(), m_screenWidth);
	m_horizontalBlurShader->Render(m_Direct3D->GetDeviceContext(), m_blurredMesh->GetIndexCount());

	m_Direct3D->TurnZBufferOn();

}

void ProceduralScene::RenderVerticalBlurTexture()
{

	XMMATRIX worldMatrix, orthoMatrix, baseViewMatrix;

	m_verticalSampleTexture->SetRenderTarget(m_Direct3D->GetDeviceContext());			// Set the render target to be the render to texture.
	m_verticalSampleTexture->ClearRenderTarget(m_Direct3D->GetDeviceContext(), 0.0f, 0.0f, 1.0f, 1.0f);		// Clear the render to texture.

	m_Camera->Update();		// Generate the view matrix based on the camera's position.

	m_Direct3D->GetWorldMatrix(worldMatrix);	// Get the world, view, and projection matrices from the camera and d3d objects.

	m_Direct3D->TurnZBufferOff();
	m_Direct3D->GetOrthoMatrix(orthoMatrix); // ortho matrix for 2D rendering
	m_Camera->GetBaseViewMatrix(baseViewMatrix);

	m_blurredMesh->SendData(m_Direct3D->GetDeviceContext());
	m_verticalBlurShader->SetShaderParameters(m_Direct3D->GetDeviceContext(), worldMatrix, baseViewMatrix, orthoMatrix, m_horizontalSampleTexture->GetShaderResourceView(), m_screenHeight);
	m_verticalBlurShader->Render(m_Direct3D->GetDeviceContext(), m_blurredMesh->GetIndexCount());
	
	m_Direct3D->TurnZBufferOn();

}

void ProceduralScene::UpSampleTexture()
{

	XMMATRIX worldMatrix, orthoMatrix, baseViewMatrix;

	m_upSampleTexture->SetRenderTarget(m_Direct3D->GetDeviceContext());		// Set the render target to be the render to texture.
	m_upSampleTexture->ClearRenderTarget(m_Direct3D->GetDeviceContext(), 0.0f, 0.0f, 1.0f, 1.0f);	// Clear the render to texture.

	m_Camera->Update();			// Generate the view matrix based on the camera's position.

	m_Direct3D->GetWorldMatrix(worldMatrix);	// Get the world, view, and projection matrices from the camera and d3d objects.

	m_Direct3D->TurnZBufferOff();
	m_Direct3D->GetOrthoMatrix(orthoMatrix);
	m_Camera->GetBaseViewMatrix(baseViewMatrix);

	m_blurredMesh->SendData(m_Direct3D->GetDeviceContext());
	m_textureShader->SetShaderParameters(m_Direct3D->GetDeviceContext(), worldMatrix, baseViewMatrix, orthoMatrix, m_verticalSampleTexture->GetShaderResourceView());
	m_textureShader->Render(m_Direct3D->GetDeviceContext(), m_blurredMesh->GetIndexCount());
	
	m_Direct3D->TurnZBufferOn();

}

void ProceduralScene::RenderGaussianBlur()
{

	DownSampleTexture();			// Down sample prior to blurring, optimise
	RenderHorizontalBlurTexture();	// Apply horizontal blur stage
	RenderVerticalBlurTexture();	// Apply vertical blur to the horizontal blur stage
	UpSampleTexture();				// Up sample, return to screen size before outputting to screen

	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, baseViewMatrix, orthoMatrix;

	m_Direct3D->SetBackBufferRenderTarget();

	m_Camera->Update();	// Generate the view matrix based on the camera's position.

	// Get the world, view, projection, and ortho matrices from the camera and Direct3D objects.
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);

	m_Direct3D->TurnZBufferOff();
	m_Direct3D->GetOrthoMatrix(orthoMatrix); // ortho matrix for 2D rendering
	m_Camera->GetBaseViewMatrix(baseViewMatrix);

	m_blurredMesh->SendData(m_Direct3D->GetDeviceContext());
	m_textureShader->SetShaderParameters(m_Direct3D->GetDeviceContext(), worldMatrix, baseViewMatrix, orthoMatrix, m_upSampleTexture->GetShaderResourceView());
	m_textureShader->Render(m_Direct3D->GetDeviceContext(), m_blurredMesh->GetIndexCount());
	
	m_Direct3D->TurnZBufferOn();

	// Present the rendered scene to the screen.
	m_Direct3D->EndScene();

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
	ProcessSphere(worldMatrix, viewMatrix, projectionMatrix, XMFLOAT3(15.0f, 30.0f, 0.0f), m_proceduralFireSphereMesh, true);

}

void ProceduralScene::RenderTheFireSphere(XMMATRIX& worldMatrix, XMMATRIX& viewMatrix, XMMATRIX& projectionMatrix)
{

	// This will process our specific sphere to the position that we want it in.
	ProcessSphere(worldMatrix, viewMatrix, projectionMatrix, XMFLOAT3(30.0f, 30.0f, 0.0f), m_normalFireSphereMesh, false);

}

void ProceduralScene::RenderTheWaterProceduralSphere(XMMATRIX& worldMatrix, XMMATRIX& viewMatrix, XMMATRIX& projectionMatrix)
{

	// This will process our specific sphere to the position that we want it in.
	ProcessSphere(worldMatrix, viewMatrix, projectionMatrix, XMFLOAT3(45.0f, 30.0f, -0.0f), m_proceduralWaterSphereMesh, true);

}

void ProceduralScene::RenderTheWaterSphere(XMMATRIX& worldMatrix, XMMATRIX& viewMatrix, XMMATRIX& projectionMatrix)
{

	// This will process our specific sphere to the position that we want it in.
	ProcessSphere(worldMatrix, viewMatrix, projectionMatrix, XMFLOAT3(60.0f, 30.0f, -0.0f), m_normalWaterSphereMesh, false);

}

void ProceduralScene::RenderTheLightningProceduralSphere(XMMATRIX& worldMatrix, XMMATRIX& viewMatrix, XMMATRIX& projectionMatrix)
{

	// This will process our specific sphere to the position that we want it in.
	ProcessSphere(worldMatrix, viewMatrix, projectionMatrix, XMFLOAT3(75.0f, 30.0f, -0.0f), m_proceduralLightningSphereMesh, true);

}

void ProceduralScene::RenderTheLightningSphere(XMMATRIX& worldMatrix, XMMATRIX& viewMatrix, XMMATRIX& projectionMatrix)
{

	// This will process our specific sphere to the position that we want it in.
	ProcessSphere(worldMatrix, viewMatrix, projectionMatrix, XMFLOAT3(90.0f, 30.0f, -0.0f), m_normalLightningSphereMesh, false);

}

void ProceduralScene::ProcessFlora(XMMATRIX& worldMatrix, XMMATRIX& viewMatrix, XMMATRIX& projectionMatrix)
{
	int currentPatch = 0;

	for (int j = 0; j < m_floraGravity.size(); j++)
	{
		for (int i = (currentPatch * MAX_AMOUNT_OF_FLORA_PER_PATCH); i < m_floraModels.size(); i++)
		{
			// If each model was scaled in the same way, this would work perfectly.
			// RenderTheFloraModel(worldMatrix, viewMatrix, projectionMatrix, m_floraModels[i], m_floraTranslations[i], m_floraID[i]);

			// If we have a tree model in our model vector.
			if (m_floraID[i] == ObjectIDNumber::ID_TREE)
			{
				// Render the tree model.
				RenderTheTreeModel(worldMatrix, viewMatrix, projectionMatrix, m_floraModels[i], m_floraTranslations[i], m_floraGravity[j]);
			}
			// Otherwise, if we have bush model in our model vector.
			else if (m_floraID[i] == ObjectIDNumber::ID_BUSH)
			{
				// Render the bush model.
				RenderTheShrubModel(worldMatrix, viewMatrix, projectionMatrix, m_floraModels[i], m_floraTranslations[i], m_floraGravity[j]);
			}
			// Otherwise, if we have bush model in our model vector.
			else if (m_floraID[i] == ObjectIDNumber::ID_GRASS)
			{
				// Render the grass model.
				RenderTheGrassModel(worldMatrix, viewMatrix, projectionMatrix, m_floraModels[i], m_floraTranslations[i], m_floraGravity[j]);
			}
		}

		currentPatch++;
	}

}

void ProceduralScene::RenderTheFloraModel(XMMATRIX& worldMatrix, XMMATRIX& viewMatrix, XMMATRIX& projectionMatrix, Model* floraModel, XMFLOAT3 translation, int IDNumber, float patchGravityValue)
{

	// The new translation, where we want our object to be.
	XMMATRIX newTransformation = XMMatrixTranslation(translation.x, translation.y, translation.z);
	XMMATRIX newRotation = XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f);
	XMMATRIX newScale = XMMatrixScaling(0.25f, 0.25f, 0.25f);

	// Multiplying the transformations together.
	worldMatrix = newScale;
	worldMatrix *= newRotation;
	worldMatrix *= newTransformation;

	// Render the tree.
	floraModel->SendData(m_Direct3D->GetDeviceContext());

	m_floraShader->SetShaderParameters(m_Direct3D->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, floraModel->GetTexture(), gravityControl + patchGravityValue, IDNumber);
	//m_floraShader->SetShaderParameters(m_Direct3D->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, floraModel->GetTexture(), m_perlinNoise->SimplexNoise(gravityDebug), ObjectIDNumber::ID_TREE);
	m_floraShader->Render(m_Direct3D->GetDeviceContext(), floraModel->GetIndexCount());

	// Reset the world matrix.
	m_Direct3D->GetWorldMatrix(worldMatrix);

}

void ProceduralScene::RenderTheTreeModel(XMMATRIX& worldMatrix, XMMATRIX& viewMatrix, XMMATRIX& projectionMatrix, Model* treeModel, XMFLOAT3 translation, float patchGravityValue)
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

	m_floraShader->SetShaderParameters(m_Direct3D->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, treeModel->GetTexture(), gravityControl + patchGravityValue, ObjectIDNumber::ID_TREE);
	//m_floraShader->SetShaderParameters(m_Direct3D->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, m_tree->GetTexture(), m_perlinNoise->SimplexNoise(gravityDebug), ObjectIDNumber::ID_TREE);
	m_floraShader->Render(m_Direct3D->GetDeviceContext(), treeModel->GetIndexCount());
	
	// Reset the world matrix.
	m_Direct3D->GetWorldMatrix(worldMatrix);

}

void ProceduralScene::RenderTheShrubModel(XMMATRIX& worldMatrix, XMMATRIX& viewMatrix, XMMATRIX& projectionMatrix, Model* shrubModel, XMFLOAT3 translation, float patchGravityValue)
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

	m_floraShader->SetShaderParameters(m_Direct3D->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, shrubModel->GetTexture(), gravityControl + patchGravityValue, ObjectIDNumber::ID_BUSH);
	//m_floraShader->SetShaderParameters(m_Direct3D->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, shrubModel->GetTexture(), m_perlinNoise->SimplexNoise(gravityDebug), ObjectIDNumber::ID_BUSH);
	m_floraShader->Render(m_Direct3D->GetDeviceContext(), shrubModel->GetIndexCount());

	// Reset the world matrix.
	m_Direct3D->GetWorldMatrix(worldMatrix);

}

void ProceduralScene::RenderTheGrassModel(XMMATRIX& worldMatrix, XMMATRIX& viewMatrix, XMMATRIX& projectionMatrix, Model* grassModel, XMFLOAT3 translation, float patchGravityValue)
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

	m_floraShader->SetShaderParameters(m_Direct3D->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, grassModel->GetTexture(), gravityControl + patchGravityValue, ObjectIDNumber::ID_GRASS);
	m_floraShader->Render(m_Direct3D->GetDeviceContext(), grassModel->GetIndexCount());

	// Reset the world matrix.
	m_Direct3D->GetWorldMatrix(worldMatrix);

}

void ProceduralScene::RenderTheGround(XMMATRIX& worldMatrix, XMMATRIX& viewMatrix, XMMATRIX& projectionMatrix)
{

	// The new translation, where we want our object to be.
	XMMATRIX new_transformation = XMMatrixTranslation(0.0f, 0.0f, -50.0f);
	XMMATRIX new_rotation = XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f);
	XMMATRIX new_scale = XMMatrixScaling(1.5f, 1.0f, 1.5f);
	//XMMATRIX new_scale = XMMatrixScaling(0.125f, 0.125f, 0.125f);

	// Multiplying the transformations together.
	worldMatrix = new_scale;
	worldMatrix *= new_rotation;
	worldMatrix *= new_transformation;

	// Rendering the plane.
	m_planeMesh->SendData(m_Direct3D->GetDeviceContext());
	m_textureShader->SetShaderParameters(m_Direct3D->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, m_planeMesh->GetTexture());
	m_textureShader->Render(m_Direct3D->GetDeviceContext(), m_planeMesh->GetIndexCount());

	// Reset the world matrix.
	m_Direct3D->GetWorldMatrix(worldMatrix);

}

void ProceduralScene::RenderTheScene(float dt, XMMATRIX& worldMatrix, XMMATRIX& viewMatrix, XMMATRIX& projectionMatrix)
{

	XMMATRIX orthoMatrix, baseViewMatrix;

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

	// Render the dirt ground.
	RenderTheGround(worldMatrix, viewMatrix, projectionMatrix);

	// Process all of the procedural flora.
	ProcessFlora(worldMatrix, viewMatrix, projectionMatrix);

	// If we want to use gaussian blur.
	if (m_activateGaussianBlur)
	{
		// Render the scene our render texture.
		RenderToTexture(dt);

		// Apply gaussian blur.
		RenderGaussianBlur();
	}

}