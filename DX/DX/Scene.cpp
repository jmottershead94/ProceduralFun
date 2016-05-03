// Scene.cpp
#include "Scene.h"

//////////////////////////////////////////////////////////
//======================================================//
//						Constructor						//
//======================================================//
// This will set up our scene and initialise all of our	//
// meshes and shaders.									//
//////////////////////////////////////////////////////////
Scene::Scene(HWND hwnd, int screenWidth, int screenHeight, D3D* direct3D, Input *in, Camera* cam, Timer* timer) : BaseScene(hwnd, screenWidth, screenHeight, direct3D, in, cam, timer)
{

	// Initialising the light control state, tessellation factor, and manipulation attributes.
	m_controllingShadowLights = true;
	m_useBoxBlur = false;
	m_tessellationFactor = 2.0f;
	m_manipulationFrequency = 0.5f;
	m_manipulationWidth = 1.0f;
	m_manipulationHeight = 1.0f;

	// Initialising the current position of the lights.
	m_currentLightPosition = { 0.0f, 0.0f, 0.0f };

	// Initialising the shadow attributes.
	for (int i = 0; i < 2; i++)
	{
		m_shadowPointLights[i] = new PointLight(m_Direct3D->GetDevice(), L"../res/DefaultDiffuse.png", XMFLOAT3(1.0f, (float)i, 0.0f), XMFLOAT3(25.0f + (i * 50.0f), 20.0f, -50.0f - (i * 10.0f)), XMFLOAT3(200.0f - (i * 200.0f), 0.0f, -50.0f));
		m_shadowTexture[i] = new RenderTexture(m_Direct3D->GetDevice(), SHADOWMAP_WIDTH, SHADOWMAP_HEIGHT, SCREEN_NEAR, SCREEN_DEPTH);
	}

	// Initialising the render textures.
	m_miniMap = new RenderTexture(m_Direct3D->GetDevice(), screenWidth, screenHeight, SCREEN_NEAR, SCREEN_DEPTH);
	m_blurredTexture = new RenderTexture(m_Direct3D->GetDevice(), screenWidth, screenHeight, SCREEN_NEAR, SCREEN_DEPTH);

	// Setting up the camera to be used with the mini-map.
	m_orthoCamera = new Camera();
	m_orthoCamera->SetPosition(m_Camera->GetPosition().x, 100.0f, m_Camera->GetPosition().z);
	m_orthoCamera->SetRotation(90.0f, 0.0f, 0.0f);

	// Initialising the meshes.
	m_model = new Model(m_Direct3D->GetDevice(), L"../res/brick1.dds", L"../res/Models/teapot.obj");
	m_miniMapDisplay = new OrthoMesh(m_Direct3D->GetDevice(), 200, 150, -300, 225);
	m_blurredScene = new OrthoMesh(m_Direct3D->GetDevice(), m_screenWidth, m_screenHeight, 0, 0);
	m_planeMesh = new PlaneMesh(m_Direct3D->GetDevice(), L"../res/White.png");
	m_water = new PlaneMesh(m_Direct3D->GetDevice(), L"../res/Water.jpg");
	m_pointLights = new PointLightGroup(m_Direct3D->GetDevice(), L"../res/DefaultDiffuse.png", XMFLOAT3(50.0f, 25.0f, 50.0f));
	m_terrain = new Terrain(m_Direct3D->GetDevice(), hwnd, L"../res/terrainDirt.jpg", L"../res/heightMapTest.png", L"../res/heightNormalTest.png");
	m_particleSystem = new ParticleSystem(m_Direct3D->GetDevice());

	// Initialising noise stuff.
	//unsigned int seed = 237;
	//m_perlinNoise = new PerlinNoise(seed);

	// Initialising the shaders.
	m_depthShader = new DepthShader(m_Direct3D->GetDevice(), hwnd);
	m_textureShader = new TextureShader(m_Direct3D->GetDevice(), hwnd);
	m_manipulationShader = new ManipulationShader(m_Direct3D->GetDevice(), hwnd);
	m_boxBlurShader = new BoxBlurShader(m_Direct3D->GetDevice(), hwnd);
	m_particleShader = new ParticleShader(m_Direct3D->GetDevice(), hwnd);
	m_masterShadowShader = new MasterShadowShader(m_Direct3D->GetDevice(), hwnd);
	m_masterShader = new MasterShader(m_Direct3D->GetDevice(), hwnd);

	// Initialising the procedural scene.
	m_proceduralScene = new ProceduralScene(hwnd, screenWidth, screenHeight, direct3D, in, cam, timer);

}

//////////////////////////////////////////////////////////
//======================================================//
//						Destructor						//
//======================================================//
// This will release all of our pointers.				//
//////////////////////////////////////////////////////////
Scene::~Scene()
{

	// Release the Direct3D objects.
	if (!m_shadowPointLights.empty())
	{
		for (auto& shadowLights : m_shadowPointLights)
		{
			delete shadowLights;
			shadowLights = nullptr;
		}
	}

	if (m_miniMap)
	{
		delete m_miniMap;
		m_miniMap = nullptr;
	}

	if (m_blurredTexture)
	{
		delete m_blurredTexture;
		m_blurredTexture = nullptr;
	}

	if (m_orthoCamera)
	{
		delete m_orthoCamera;
		m_orthoCamera = nullptr;
	}

	if (m_model)
	{
		delete m_model;
		m_model = nullptr;
	}
	
	if (m_miniMapDisplay)
	{
		delete m_miniMapDisplay;
		m_miniMapDisplay = nullptr;
	}

	if (m_blurredScene)
	{
		delete m_blurredScene;
		m_blurredScene = nullptr;
	}

	if (m_planeMesh)
	{
		delete m_planeMesh;
		m_planeMesh = nullptr;
	}

	if (m_water)
	{
		delete m_water;
		m_water = nullptr;
	}

	if (m_pointLights)
	{
		delete m_pointLights;
		m_pointLights = nullptr;
	}

	if (m_terrain)
	{
		delete m_terrain;
		m_terrain = nullptr;
	}

	if (m_particleSystem)
	{
		delete m_particleSystem;
		m_particleSystem = nullptr;
	}

	if (m_depthShader)
	{
		delete m_depthShader;
		m_depthShader = nullptr;
	}

	if (m_textureShader)
	{
		delete m_textureShader;
		m_textureShader = nullptr;
	}

	if (m_manipulationShader)
	{
		delete m_manipulationShader;
		m_manipulationShader = nullptr;
	}

	if (m_boxBlurShader)
	{
		delete m_boxBlurShader;
		m_boxBlurShader = nullptr;
	}

	if (m_particleShader)
	{
		delete m_particleShader;
		m_particleShader = nullptr;
	}

	if (m_masterShadowShader)
	{
		delete m_masterShadowShader;
		m_masterShadowShader = nullptr;
	}

	if (m_masterShader)
	{
		delete m_masterShader;
		m_masterShader = nullptr;
	}

	if (m_proceduralScene)
	{
		// Clean up the procedural scene.
		m_proceduralScene->~ProceduralScene();

		// Delete the procedural scene pointer.
		delete m_proceduralScene;
		m_proceduralScene = nullptr;
	}

}

//////////////////////////////////////////////////////////
//======================================================//
//						Controls.						//
//======================================================//
// This will provide us with the ability to manipulate	//
// the scene.											//
//////////////////////////////////////////////////////////
void Scene::Controls(float dt)
{
	m_proceduralScene->Controls(dt);

	// Terrain Light controls.
	if (m_Input->isKeyDown(VK_NUMPAD6))
	{
		m_currentLightPosition.x += (1.0f * dt);
	}
	else if (m_Input->isKeyDown(VK_NUMPAD4))
	{
		m_currentLightPosition.x -= (1.0f * dt);
	}
	else
	{
		m_currentLightPosition.x = 0.0f;
	}

	if (m_Input->isKeyDown(VK_NUMPAD7))
	{
		m_currentLightPosition.y += (1.0f * dt);
	}
	else if (m_Input->isKeyDown(VK_NUMPAD9))
	{
		m_currentLightPosition.y -= (1.0f * dt);
	}
	else
	{
		m_currentLightPosition.y = 0.0f;
	}

	if (m_Input->isKeyDown(VK_NUMPAD8))
	{
		m_currentLightPosition.z += (1.0f * dt);
	}
	else if (m_Input->isKeyDown(VK_NUMPAD5))
	{
		m_currentLightPosition.z -= (1.0f * dt);
	}
	else
	{
		m_currentLightPosition.z = 0.0f;
	}

	// Changing current controlling light.
	if (m_Input->isKeyDown('Z'))
	{
		// We will not control shadow point lights.
		m_controllingShadowLights = true;
	}
	else if (m_Input->isKeyDown('X'))
	{
		// We will not control terrain point lights.
		m_controllingShadowLights = false;
	}

	// Changing blur filters.
	if (m_Input->isKeyDown('1'))
	{
		// Use box blur.
		m_useBoxBlur = true;
	}
	// If we want to revert to our normal scene.
	else if (m_Input->isKeyDown('0'))
	{
		// Change all of the blur states off.
		m_useBoxBlur = false;
	}

	// Increasing/Decreasing tessellation.
	if (m_Input->isKeyDown(VK_ADD) && m_tessellationFactor < 9.0f)
	{
		m_tessellationFactor++;
	}
	else if (m_Input->isKeyDown(VK_SUBTRACT) && m_tessellationFactor > 1.0f)
	{
		m_tessellationFactor--;
	}

	// Increasing/Decreasing wave frequency.
	if (m_Input->isKeyDown('I') )
	{
		m_manipulationFrequency += 0.01f;
	}
	else if (m_Input->isKeyDown('K') && m_manipulationFrequency > 0.01f)
	{
		m_manipulationFrequency -= 0.01f;
	}

}

//////////////////////////////////////////////////////////
//======================================================//
//					RenderToTextureBlur					//
//======================================================//
// Here I will render the scene to a texture in order	//
// to use the basic box blur.							//
//////////////////////////////////////////////////////////
void Scene::RenderToTextureBlur()
{

	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;

	// Set the render target to be the render to texture.
	m_blurredTexture->SetRenderTarget(m_Direct3D->GetDeviceContext());

	// Clear the render to the texture.
	m_blurredTexture->ClearRenderTarget(m_Direct3D->GetDeviceContext(), 0.39f, 0.58f, 0.92f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->Update();

	// Get the world, view, projection, and ortho matrices from the camera and Direct3D objects.
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);

	// Rendering a teapot model.
	RenderTheModel(worldMatrix, viewMatrix, projectionMatrix);

	// Rendering a plane mesh.
	RenderTheFloor(worldMatrix, viewMatrix, projectionMatrix);

	// Rendering the point lights and sphere meshes.
	RenderThePointLights(worldMatrix, viewMatrix, projectionMatrix);

	// Rendering the shadow point lights.
	RenderTheShadowPointLights(worldMatrix, viewMatrix, projectionMatrix);

	// Rendering the terrain.
	RenderTheTerrain(worldMatrix, viewMatrix, projectionMatrix);

	// Rendering the wavey water.
	RenderTheWater(worldMatrix, viewMatrix, projectionMatrix);

	// Rendering the particles.
	RenderTheParticles(worldMatrix, viewMatrix, projectionMatrix);

	// Reset the render target back to the original back buffer and not the render to texture anymore.
	m_Direct3D->SetBackBufferRenderTarget();

}

//////////////////////////////////////////////////////////
//======================================================//
//					RenderToShadowTexture				//
//======================================================//
// Here I will render the scene to a texture in order	//
// to store the shadows.								//
//////////////////////////////////////////////////////////
void Scene::RenderToShadowTexture()
{

	XMMATRIX worldMatrix, viewMatrix, lightViewMatrix, lightProjectionMatrix;

	for (int i = 0; i < 2; i++)
	{
		// Set the render target to be the render to texture.
		m_shadowTexture[i]->SetRenderTarget(m_Direct3D->GetDeviceContext());

		// Clear the render to the texture.
		m_shadowTexture[i]->ClearRenderTarget(m_Direct3D->GetDeviceContext(), 0.0f, 0.0f, 0.0f, 1.0f);

		// Generating a view for the light.
		m_shadowPointLights[i]->GetPointLight()->GenerateViewMatrix();
		m_shadowPointLights[i]->GetPointLight()->GenerateProjectionMatrix(SCREEN_NEAR, SCREEN_DEPTH);

		// Get the world, view, and projection from the light and direct 3D objects.
		m_Direct3D->GetWorldMatrix(worldMatrix);
		lightViewMatrix = m_shadowPointLights[i]->GetPointLight()->GetViewMatrix();
		lightProjectionMatrix = m_shadowPointLights[i]->GetPointLight()->GetProjectionMatrix();

		// Translating the teapot model.
		worldMatrix = XMMatrixTranslation(50.0f, 8.0f, -50.0f);

		// Geometry data for the teapot.
		m_model->SendData(m_Direct3D->GetDeviceContext());

		// Shader data for the teapot.
		m_depthShader->SetShaderParameters(m_Direct3D->GetDeviceContext(), worldMatrix, lightViewMatrix, lightProjectionMatrix);
		m_depthShader->Render(m_Direct3D->GetDeviceContext(), m_model->GetIndexCount());

		// Reset the world matrix.
		m_Direct3D->GetWorldMatrix(worldMatrix);

		// Translating the plane into the view of the camera.
		worldMatrix = XMMatrixTranslation(0.0f, 0.0f, -100.0f);

		// Geometry data for the plane.
		m_planeMesh->SendData(m_Direct3D->GetDeviceContext());

		// Shader data for the plane.
		m_depthShader->SetShaderParameters(m_Direct3D->GetDeviceContext(), worldMatrix, lightViewMatrix, lightProjectionMatrix);
		m_depthShader->Render(m_Direct3D->GetDeviceContext(), m_planeMesh->GetIndexCount());

		// Reset the world matrix.
		m_Direct3D->GetWorldMatrix(worldMatrix);

		// Reset the render target back to the original back buffer and not the render to texture anymore.
		m_Direct3D->SetBackBufferRenderTarget();

		// Resetting the viewport.
		m_Direct3D->ResetViewport();

		// Store the depth map.
		m_depthMaps[i] = m_shadowTexture[i]->GetShaderResourceView();
	}

}

//////////////////////////////////////////////////////////
//======================================================//
//					RenderToMiniMap						//
//======================================================//
// Here I will render the scene to a texture in order	//
// to create a mini map.								//
//////////////////////////////////////////////////////////
void Scene::RenderToMiniMap()
{

	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;

	// Set the render target to be the render to texture.
	m_miniMap->SetRenderTarget(m_Direct3D->GetDeviceContext());

	// Clear the render to the texture.
	m_miniMap->ClearRenderTarget(m_Direct3D->GetDeviceContext(), 0.0f, 1.0f, 0.0f, 1.0f);

	// Setting a top down view of the scene in the minimap.
	m_orthoCamera->SetPosition(m_Camera->GetPosition().x, m_orthoCamera->GetPosition().y, m_Camera->GetPosition().z);

	// Generate the view matrix based on the camera's position.
	m_orthoCamera->Update();
	
	// Get the world, view, projection, and ortho matrices from the camera and Direct3D objects.
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_orthoCamera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);

	// Rendering a teapot model.
	RenderTheModel(worldMatrix, viewMatrix, projectionMatrix);

	// Rendering a plane mesh.
	RenderTheFloor(worldMatrix, viewMatrix, projectionMatrix);

	// Rendering the point lights and sphere meshes.
	RenderThePointLights(worldMatrix, viewMatrix, projectionMatrix);

	// Rendering the shadow point lights.
	RenderTheShadowPointLights(worldMatrix, viewMatrix, projectionMatrix);

	// Rendering the terrain.
	RenderTheTerrain(worldMatrix, viewMatrix, projectionMatrix);

	// Rendering the wavey water.
	RenderTheWater(worldMatrix, viewMatrix, projectionMatrix);

	// Rendering the particles.
	RenderTheParticles(worldMatrix, viewMatrix, projectionMatrix);

	// Reset the render target back to the original back buffer and not the render to texture anymore.
	m_Direct3D->SetBackBufferRenderTarget();

}

//////////////////////////////////////////////////////////
//======================================================//
//					RenderTheModel						//
//======================================================//
// This will render the model for our scene.			//
//////////////////////////////////////////////////////////
void Scene::RenderTheModel(XMMATRIX& worldMatrix, XMMATRIX& viewMatrix, XMMATRIX& projectionMatrix)
{

	// Moving the model where I want it to be.
	worldMatrix = XMMatrixTranslation(50.0f, 8.0f, -50.0f);

	// Rendering the model.
	m_model->SendData(m_Direct3D->GetDeviceContext());
	m_masterShadowShader->SetShaderParameters(m_Direct3D->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, m_model->GetTexture(), m_depthMaps, m_shadowPointLights);
	m_masterShadowShader->Render(m_Direct3D->GetDeviceContext(), m_model->GetIndexCount());

	// Reset the world matrix.
	m_Direct3D->GetWorldMatrix(worldMatrix);

}

//////////////////////////////////////////////////////////
//======================================================//
//					RenderTheFloor						//
//======================================================//
// This will render the floor for our scene.			//
//////////////////////////////////////////////////////////
void Scene::RenderTheFloor(XMMATRIX& worldMatrix, XMMATRIX& viewMatrix, XMMATRIX& projectionMatrix)
{

	// Moving the plane where I want it to be.
	worldMatrix = XMMatrixTranslation(0.0f, 0.0f, -100.0f);

	// Rendering the plane.
	m_planeMesh->SendData(m_Direct3D->GetDeviceContext());
	m_masterShadowShader->SetShaderParameters(m_Direct3D->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, m_planeMesh->GetTexture(), m_depthMaps, m_shadowPointLights);
	m_masterShadowShader->Render(m_Direct3D->GetDeviceContext(), m_planeMesh->GetIndexCount());

	// Reset the world matrix.
	m_Direct3D->GetWorldMatrix(worldMatrix);

}

//////////////////////////////////////////////////////////
//======================================================//
//				RenderThePointLights					//
//======================================================//
// This will render the point lights for our scene.		//
// These point lights will be able to interact with our	//
// terrain.												//
//////////////////////////////////////////////////////////
void Scene::RenderThePointLights(XMMATRIX& worldMatrix, XMMATRIX& viewMatrix, XMMATRIX& projectionMatrix)
{

	// A float 3 for our new point light position.
	XMFLOAT3 newPositions = { 0.0f, 0.0f, 0.0f };

	// Render all of the point lights with their spheres in the correct position.
	for (int i = 0; i < 3; i++)
	{
		if (m_controllingShadowLights)
		{
			// Translate the world matrix.
			worldMatrix = XMMatrixTranslation(m_pointLights->GetPointLights()[i]->GetPointLight()->GetPosition().x, m_pointLights->GetPointLights()[i]->GetPointLight()->GetPosition().y, m_pointLights->GetPointLights()[i]->GetPointLight()->GetPosition().z);
		}
		else
		{
			// Calculating a new position for the point lights.
			newPositions = { (m_pointLights->GetPointLights()[i]->GetPointLight()->GetPosition().x + m_currentLightPosition.x), (m_pointLights->GetPointLights()[i]->GetPointLight()->GetPosition().y + m_currentLightPosition.y), (m_pointLights->GetPointLights()[i]->GetPointLight()->GetPosition().z + m_currentLightPosition.z) };

			// Setting up the translation matrix.
			worldMatrix = XMMatrixTranslation(newPositions.x, newPositions.y, newPositions.z);
			m_pointLights->GetPointLights()[i]->GetPointLight()->SetPosition(newPositions.x, newPositions.y, newPositions.z);
		}

		// Rendering the point light.
		m_pointLights->GetPointLights()[i]->GetSphereMesh()->SendData(m_Direct3D->GetDeviceContext());
		m_textureShader->SetShaderParameters(m_Direct3D->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, m_pointLights->GetPointLights()[i]->GetSphereMesh()->GetTexture());
		m_textureShader->Render(m_Direct3D->GetDeviceContext(), m_pointLights->GetPointLights()[i]->GetSphereMesh()->GetIndexCount());

		// Reset the world matrix.
		m_Direct3D->GetWorldMatrix(worldMatrix);
	}

}

//////////////////////////////////////////////////////////
//======================================================//
//				RenderTheShadowPointLights				//
//======================================================//
// This will render the point lights that will be used	//
// for showing off multiple shadows.					//
//////////////////////////////////////////////////////////
void Scene::RenderTheShadowPointLights(XMMATRIX& worldMatrix, XMMATRIX& viewMatrix, XMMATRIX& projectionMatrix)
{

	// A float3 for storing our new position.
	XMFLOAT3 newPositions = { 0.0f, 0.0f, 0.0f };

	// Render all of the point lights with their spheres in the correct position.
	for (int i = 0; i < 2; i++)
	{
		if (m_controllingShadowLights)
		{
			// Calculating a new position for the point lights.
			newPositions = { (m_shadowPointLights[i]->GetPointLight()->GetPosition().x + m_currentLightPosition.x), (m_shadowPointLights[i]->GetPointLight()->GetPosition().y + m_currentLightPosition.y), (m_shadowPointLights[i]->GetPointLight()->GetPosition().z + m_currentLightPosition.z) };

			// Setting up the translation matrix.
			worldMatrix = XMMatrixTranslation(newPositions.x, newPositions.y, newPositions.z);
			m_shadowPointLights[i]->GetPointLight()->SetPosition(newPositions.x, newPositions.y, newPositions.z);
		}
		else
		{
			// Translate the world matrix.
			worldMatrix = XMMatrixTranslation(m_shadowPointLights[i]->GetPointLight()->GetPosition().x, m_shadowPointLights[i]->GetPointLight()->GetPosition().y, m_shadowPointLights[i]->GetPointLight()->GetPosition().z);
		}

		// Rendering the point lights.
		m_shadowPointLights[i]->GetSphereMesh()->SendData(m_Direct3D->GetDeviceContext());
		m_textureShader->SetShaderParameters(m_Direct3D->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, m_shadowPointLights[i]->GetSphereMesh()->GetTexture());
		m_textureShader->Render(m_Direct3D->GetDeviceContext(), m_shadowPointLights[i]->GetSphereMesh()->GetIndexCount());

		// Reset the world matrix.
		m_Direct3D->GetWorldMatrix(worldMatrix);
	}

}

//////////////////////////////////////////////////////////
//======================================================//
//					RenderTheTerrain					//
//======================================================//
// This will render the terrain for our scene.			//
//////////////////////////////////////////////////////////
void Scene::RenderTheTerrain(XMMATRIX& worldMatrix, XMMATRIX& viewMatrix, XMMATRIX& projectionMatrix)
{

	// Render the terrain.
	m_terrain->SendData(m_Direct3D->GetDeviceContext());
	m_masterShader->SetShaderParameters(m_Direct3D->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, m_terrain->GetTexture(), m_terrain->GetHeightMap(), m_terrain->GetNormalMap(), m_tessellationFactor, m_pointLights);
	m_masterShader->Render(m_Direct3D->GetDeviceContext(), m_terrain->GetIndexCount());
	
}

//////////////////////////////////////////////////////////
//======================================================//
//					RenderTheWater						//
//======================================================//
// This will render the water for our scene.			//
//////////////////////////////////////////////////////////
void Scene::RenderTheWater(XMMATRIX& worldMatrix, XMMATRIX& viewMatrix, XMMATRIX& projectionMatrix)
{

	// Moving the water where I want it to be.
	worldMatrix = XMMatrixTranslation(0.0f, 5.0f, 0.0f);

	// Used for a transparency effect, the water will be semi-transparent.
	m_Direct3D->TurnOnAlphaBlending();

	// Rendering the water.
	m_water->SendData(m_Direct3D->GetDeviceContext());
	m_manipulationShader->SetShaderParameters(m_Direct3D->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, m_water->GetTexture(), nullptr, m_Timer, m_manipulationFrequency, m_manipulationHeight, m_manipulationWidth);
	m_manipulationShader->Render(m_Direct3D->GetDeviceContext(), m_water->GetIndexCount());

	// Turning off transparency.
	m_Direct3D->TurnOffAlphaBlending();

	// Reset the world matrix.
	m_Direct3D->GetWorldMatrix(worldMatrix);

}

//////////////////////////////////////////////////////////
//======================================================//
//					RenderTheParticles					//
//======================================================//
// This will render the particles for our scene.		//
//////////////////////////////////////////////////////////
void Scene::RenderTheParticles(XMMATRIX& worldMatrix, XMMATRIX& viewMatrix, XMMATRIX& projectionMatrix)
{

	// Looping through our particle system.
	for (int i = 0; i < m_particleSystem->GetParticles().size(); i++)
	{
		// Rendering the particles.
		m_particleSystem->GetParticles()[i]->SendData(m_Direct3D->GetDeviceContext());
		m_particleShader->SetShaderParameters(m_Direct3D->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, NULL, XMFLOAT3(50.0f, 0.0f, -65.0f));
		m_particleShader->Render(m_Direct3D->GetDeviceContext(), m_particleSystem->GetParticles()[i]->GetIndexCount());
	}

}

//////////////////////////////////////////////////////////
//======================================================//
//					RenderTheScene						//
//======================================================//
// This will put everything together for our scene.		//
//////////////////////////////////////////////////////////
void Scene::RenderTheScene(float dt)
{

	// Post processing/Multi-pass rendering.
	//RenderToShadowTexture();
	//RenderToMiniMap();
	RenderToTextureBlur();

	XMMATRIX worldMatrix, baseViewMatrix, viewMatrix, projectionMatrix, orthoMatrix;

	// Clear the scene. (blue colour).
	m_Direct3D->BeginScene(0.39f, 0.58f, 0.92f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->Update();

	// Get the world, view, projection, and ortho matrices from the camera and Direct3D objects.
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);

	// Handling user input.
	Controls(dt);

	// Rendering a teapot model.
	//RenderTheModel(worldMatrix, viewMatrix, projectionMatrix);

	// Rendering a plane mesh.
	RenderTheFloor(worldMatrix, viewMatrix, projectionMatrix);	

	// Rendering the point lights and sphere meshes.
	RenderThePointLights(worldMatrix, viewMatrix, projectionMatrix);

	// Rendering the shadow point lights.
	RenderTheShadowPointLights(worldMatrix, viewMatrix, projectionMatrix);

	// Rendering the terrain.
	//RenderTheTerrain(worldMatrix, viewMatrix, projectionMatrix);

	// Rendering the wavey water.
	//RenderTheWater(worldMatrix, viewMatrix, projectionMatrix);

	// Rendering the particles.
	//RenderTheParticles(worldMatrix, viewMatrix, projectionMatrix);

	// Render the procedural methods scene.
	m_proceduralScene->RenderTheScene(dt, worldMatrix, viewMatrix, projectionMatrix);

	// To render ortho mesh.
	// Turn off the z buffer to begin all 2D rendering.
	m_Direct3D->TurnZBufferOff();

	// Ortho Matrix for 2D rendering.
	m_Direct3D->GetOrthoMatrix(orthoMatrix);	
	m_Camera->GetBaseViewMatrix(baseViewMatrix);
	
	// Displaying the mini map.
	m_miniMapDisplay->SendData(m_Direct3D->GetDeviceContext());
	m_textureShader->SetShaderParameters(m_Direct3D->GetDeviceContext(), worldMatrix, baseViewMatrix, orthoMatrix, m_miniMap->GetShaderResourceView());
	m_textureShader->Render(m_Direct3D->GetDeviceContext(), m_miniMapDisplay->GetIndexCount());

	// If we want to use box blur.
	if (m_useBoxBlur)
	{
		// Apply box blur.
		m_blurredScene->SendData(m_Direct3D->GetDeviceContext());
		m_boxBlurShader->SetShaderParameters(m_Direct3D->GetDeviceContext(), worldMatrix, baseViewMatrix, orthoMatrix, m_blurredTexture->GetShaderResourceView(), m_screenWidth, m_screenHeight);
		m_boxBlurShader->Render(m_Direct3D->GetDeviceContext(), m_blurredScene->GetIndexCount());
	}

	// Turn on the z buffer to render back to the 3D scene.
	m_Direct3D->TurnZBufferOn();
	
	// Present the rendered scene to the screen.
	m_Direct3D->EndScene();

}