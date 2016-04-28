// Jason Mottershead, 1300455.

// Base scene class header file.
// This class will help to split up applications from scenes.
// This will provide base pointers and common access to any inheriting scenes.

// Header guard.
#ifndef _PROCEDURALSCENE_H
#define _PROCEDURALSCENE_H

// Includes
// DirectX.
#include "d3d.h"

// System.
#include "Input.h"
#include "Camera.h"
#include "BaseScene.h"
#include "Timer.h"

// Geometry.
#include "SphereMesh.h"

// Shaders.
#include "PerlinNoiseShader.h"

class ProceduralScene : public BaseScene
{

	public:
		// Methods.
		ProceduralScene(HWND hwnd, int screenWidth, int screenHeight, D3D* direct3D, Input* in, Camera* cam, Timer* timer);
		~ProceduralScene();
		void RenderTheScene(float dt, XMMATRIX& worldMatrix, XMMATRIX& viewMatrix, XMMATRIX& projectionMatrix);
		void RenderTheSphere(XMMATRIX& worldMatrix, XMMATRIX& viewMatrix, XMMATRIX& projectionMatrix);

		// Setters.
		// Setting the current sphere rotation.
		inline void SetSphereRotation(float rotation) { sphereRotation += rotation; }

	protected:
		// Attributes.
		// Standard.
		float sphereRotation;

		// Geometry.
		SphereMesh* m_sphereMesh;													// The sphere mesh that we will be applying the perlin noise texture to.

		// Shaders.
		PerlinNoiseShader* m_perlinNoiseShader;										// The shader that handles perlin noise processing.
		
};

#endif