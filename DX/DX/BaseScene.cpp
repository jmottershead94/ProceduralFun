// Base scene.cpp
#include "BaseScene.h"

//////////////////////////////////////////////////////////
//======================================================//
//						Constructor						//
//======================================================//
// This will initialise all of our base scene pointers.	//
//////////////////////////////////////////////////////////
BaseScene::BaseScene(HWND hwnd, int screenWidth, int screenHeight, D3D* direct3D, Input *in, Camera* cam, Timer* timer)
{

	// Initialising base scene attributes.
	m_Hwnd = hwnd;
	m_Direct3D = direct3D;
	m_Input = in;
	m_Camera = cam;
	m_Timer = timer;
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;

}