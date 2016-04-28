// Jason Mottershead, 1300455.

// Base scene class header file.
// This class will help to split up applications from scenes.
// This will provide base pointers and common access to any inheriting scenes.

// Header guard.
#ifndef _BASESCENE_H
#define _BASESCENE_H

// Includes
// DirectX.
#include "d3d.h"

// System.
#include "Input.h"
#include "Camera.h"
#include "Timer.h"

class BaseScene
{

	public:
		// Methods.
		void Controls(float dt);
		void RenderTheScene(float dt);

	protected:
		// Attributes.
		const int SHADOWMAP_WIDTH = 1024;	// Used for shadow mapping.
		const int SHADOWMAP_HEIGHT = 1024;
		const float SCREEN_NEAR = 0.1f;		// Used for depth sorting.
		const float SCREEN_DEPTH = 100.0f;	
		int m_screenWidth, m_screenHeight;	// Access to the screen width and height.
		HWND m_Hwnd;						// Handle to our window.
		D3D* m_Direct3D;					// Access to Direct3D.
		Input* m_Input;						// Access to input.
		Camera* m_Camera;					// Access to the camera.
		Timer* m_Timer;						// Access to timer, and dt.

		// Methods.
		BaseScene(HWND hwnd, int screenWidth, int screenHeight, D3D* direct3D, Input* in, Camera* cam, Timer* timer);

			

};

#endif