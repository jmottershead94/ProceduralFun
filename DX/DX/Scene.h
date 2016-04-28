// Jason Mottershead, 1300455.

// Scene class header file.
// This class will help separate applications and scenes.
// It will also set the scene for my coursework and bring everything together.

// Header guard.
#ifndef _SCENE_H
#define _SCENE_H

// Includes
// System.
#include "Input.h"
#include "RenderTexture.h"
#include "BaseScene.h"
#include "PerlinNoise.h"

// Geometry.
#include "Model.h"
#include "OrthoMesh.h"
#include "PlaneMesh.h"
#include "PointLightGroup.h"
#include "Terrain.h"
#include "ParticleSystem.h"
#include "NoisePlane.h"
#include "SphereMesh.h"

// Shaders.
#include "DepthShader.h"
#include "TextureShader.h"
#include "ManipulationShader.h"
#include "BoxBlurShader.h"
#include "ParticleShader.h"
#include "MasterShadowShader.h"
#include "MasterShader.h"
#include "PerlinNoiseShader.h"

// Scene IS A Base Scene, therefore inherits from it.
class Scene : protected BaseScene
{
	public:
		// Methods.
		Scene(HWND hwnd, int screenWidth, int screenHeight, D3D* direct3D, Input* in, Camera* cam, Timer* timer);
		~Scene();
		void RenderTheScene(float dt);

	private:
		// Attributes.
		// Standard library.
		bool m_controllingShadowLights, m_useBoxBlur;								// Flags for different controls.
		float m_tessellationFactor;													// The tessellation factor for the terrain.
		float m_manipulationFrequency, m_manipulationWidth, m_manipulationHeight;	// Manipulation factors for the water.
		std::array<PointLight*, 2> m_shadowPointLights;								// The point lights used to show off multiple shadows.

		// DirectX.
		XMFLOAT3 m_currentLightPosition;											// Our current translation position for the point lights.
		ID3D11ShaderResourceView* m_depthMaps[2];									// The depth maps used for multiple shadows.

		// System.
		RenderTexture* m_shadowTexture[2];											// Our multiple shadow textures.
		RenderTexture* m_miniMap;													// The render texture for the mini map display.
		RenderTexture* m_blurredTexture;											// The render texture for the blurred scene.
		Camera* m_orthoCamera;														// The camera to show our position on the mini map.

		// Geometry.
		Model* m_model;																// The model I am loading in for shadows.
		OrthoMesh* m_miniMapDisplay;												// The ortho mesh used for displaying the mini map.
		OrthoMesh* m_blurredScene;													// The ortho mesh used for displaying the blurred scene.
		PlaneMesh* m_planeMesh;														// The plane mesh for showing off multiple shadows.
		PlaneMesh* m_water;															// The plane mesh for showing off the manipulation shader.										
		PointLightGroup* m_pointLights;												// The group of point lights used to interact with the terrain.
		Terrain* m_terrain;															// The terrain mesh used to show off height and normal maps, as well as tessellation.
		ParticleSystem* m_particleSystem;											// The particle system mesh used to show off the geometry shader.
		SphereMesh* m_sphereMesh;													// The sphere mesh that we will be applying the perlin noise texture to.

		// Shaders.
		DepthShader* m_depthShader;													// The depth shader used for shadows,
		TextureShader* m_textureShader;												// The texture shader used for displaying the spheres for our point lights.
		ManipulationShader* m_manipulationShader;									// The manipulation shader for making our water plane mesh wavey.
		BoxBlurShader* m_boxBlurShader;												// The box blur shader for blurring our entire scene - for post processing.
		ParticleShader* m_particleShader;											// The particle shader used for rendering and sorting out our particles.
		MasterShadowShader* m_masterShadowShader;									// The master shadow shader for dealing with multiple shadows.
		MasterShader* m_masterShader;												// The master shader for dealing with point lights, as well as tessellation.
		PerlinNoiseShader* m_perlinNoiseShader;										// The shader that handles perlin noise processing.

		// Testing Noise.
		NoisePlane* m_noisePlane;											// The perlin noise plane.
		float sphereRotation;

		// Methods.
		void Controls(float dt);
		void RenderToTextureBlur();
		void RenderToShadowTexture();
		void RenderToMiniMap();
		void RenderTheModel(XMMATRIX& worldMatrix, XMMATRIX& viewMatrix, XMMATRIX& projectionMatrix);
		void RenderTheFloor(XMMATRIX& worldMatrix, XMMATRIX& viewMatrix, XMMATRIX& projectionMatrix);
		void RenderThePointLights(XMMATRIX& worldMatrix, XMMATRIX& viewMatrix, XMMATRIX& projectionMatrix);
		void RenderTheShadowPointLights(XMMATRIX& worldMatrix, XMMATRIX& viewMatrix, XMMATRIX& projectionMatrix);
		void RenderTheTerrain(XMMATRIX& worldMatrix, XMMATRIX& viewMatrix, XMMATRIX& projectionMatrix);
		void RenderTheWater(XMMATRIX& worldMatrix, XMMATRIX& viewMatrix, XMMATRIX& projectionMatrix);
		void RenderTheParticles(XMMATRIX& worldMatrix, XMMATRIX& viewMatrix, XMMATRIX& projectionMatrix);
		void RenderTheNoisePlane(XMMATRIX& worldMatrix, XMMATRIX& viewMatrix, XMMATRIX& projectionMatrix);
		void RenderTheSphere(XMMATRIX& worldMatrix, XMMATRIX& viewMatrix, XMMATRIX& projectionMatrix);

};

#endif