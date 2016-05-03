// Jason Mottershead, 1300455.

// Base scene class header file.
// This class will help to split up applications from scenes.
// This will provide base pointers and common access to any inheriting scenes.

// Header guard.
#ifndef _PROCEDURALSCENE_H
#define _PROCEDURALSCENE_H

// Includes
#include <time.h>

// DirectX.
#include "d3d.h"

// System.
#include "Input.h"
#include "Camera.h"
#include "BaseScene.h"
#include "Timer.h"

// Geometry.
#include "SphereMesh.h"
#include "Model.h"

// Shaders.
#include "TextureShader.h"
#include "PulsingPlanetShader.h"
#include "FloraShader.h"

// Perlin Noise.
#include "PerlinNoise.h"
#include "SimplexNoise.h"

#include "ObjectIDNumber.h"

class ProceduralScene : public BaseScene
{

	public:
		// Methods.
		ProceduralScene(HWND hwnd, int screenWidth, int screenHeight, D3D* direct3D, Input* in, Camera* cam, Timer* timer);
		~ProceduralScene();
		void RenderTheScene(float dt, XMMATRIX& worldMatrix, XMMATRIX& viewMatrix, XMMATRIX& projectionMatrix);
		void Controls(float dt);
		

		// Setters.
		// Setting the current sphere rotation.
		inline void SetSphereRotation(float rotation) { sphereRotation += rotation; }

	protected:
		// Attributes.
		// Standard.
		const int MAX_AMOUNT_OF_FLORA = 256;
		const int MAX_AMOUNT_OF_FLORA_PER_PATCH = 32;
		float sphereRotation;
		XMFLOAT3 treeRotation;
		float gravityDebug;
		int noiseIDValue;
		int noiseIDValue2;

		// Geometry.
		SphereMesh* m_proceduralFireSphereMesh;			// The sphere mesh that we will be applying the perlin noise texture to.
		SphereMesh* m_normalFireSphereMesh;				// The sphere mesh that we will show off the original texture.
		SphereMesh* m_proceduralWaterSphereMesh;		// The sphere mesh that we will be applying the perlin noise texture to.
		SphereMesh* m_normalWaterSphereMesh;			// The sphere mesh that we will show off the original texture.
		SphereMesh* m_proceduralLightningSphereMesh;	// The sphere mesh that we will be applying the perlin noise texture to.
		SphereMesh* m_normalLightningSphereMesh;		// The sphere mesh that we will show off the original texture.
		Model* m_tree;
		Model* m_shrub;
		Model* m_grass;
		Model* m_NOTGRASS;
		vector<Model*> m_floraModels;
		vector<XMFLOAT3> m_floraTranslations;
		vector<int> m_floraID;

		// Shaders.
		TextureShader* m_textureShader;				// This will be used to show off the original texture.
		PulsingPlanetShader* m_perlinNoiseShader;	// The shader that handles perlin noise processing.
		FloraShader* m_floraShader;					// This shader will handle the procedurally generated flora.

		// Perlin Noise.
		PerlinNoise* m_perlinNoise;
		SimplexNoise* m_simplexNoise;

	private:
		// Methods.
		void InitialiseFlora(XMFLOAT3 newStartPosition);
		void RemoveFlora();
		void ProcessSphere(XMMATRIX& worldMatrix, XMMATRIX& viewMatrix, XMMATRIX& projectionMatrix, XMFLOAT3 position, SphereMesh* sphereMesh, bool isProcedural);
		void RenderTheFireProceduralSphere(XMMATRIX& worldMatrix, XMMATRIX& viewMatrix, XMMATRIX& projectionMatrix);
		void RenderTheFireSphere(XMMATRIX& worldMatrix, XMMATRIX& viewMatrix, XMMATRIX& projectionMatrix);
		void RenderTheWaterProceduralSphere(XMMATRIX& worldMatrix, XMMATRIX& viewMatrix, XMMATRIX& projectionMatrix);
		void RenderTheWaterSphere(XMMATRIX& worldMatrix, XMMATRIX& viewMatrix, XMMATRIX& projectionMatrix);
		void RenderTheLightningProceduralSphere(XMMATRIX& worldMatrix, XMMATRIX& viewMatrix, XMMATRIX& projectionMatrix);
		void RenderTheLightningSphere(XMMATRIX& worldMatrix, XMMATRIX& viewMatrix, XMMATRIX& projectionMatrix);
		void ProcessFlora(XMMATRIX& worldMatrix, XMMATRIX& viewMatrix, XMMATRIX& projectionMatrix);
		void RenderTheFloraModel(XMMATRIX& worldMatrix, XMMATRIX& viewMatrix, XMMATRIX& projectionMatrix, Model* floraModel, XMFLOAT3 translation, int IDNumber);
		void RenderTheTreeModel(XMMATRIX& worldMatrix, XMMATRIX& viewMatrix, XMMATRIX& projectionMatrix, Model* treeModel, XMFLOAT3 translation);
		void RenderTheShrubModel(XMMATRIX& worldMatrix, XMMATRIX& viewMatrix, XMMATRIX& projectionMatrix, Model* shrubModel, XMFLOAT3 translation);
		void RenderTheGrassModel(XMMATRIX& worldMatrix, XMMATRIX& viewMatrix, XMMATRIX& projectionMatrix, Model* grassModel, XMFLOAT3 translation);
};

#endif