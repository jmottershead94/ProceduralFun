// Coursework.cpp
#include "Coursework.h"

//////////////////////////////////////////////////////////
//======================================================//
//						Constructor						//
//======================================================//
// This will initialise our application and set the		//
// scene, with camera pointing in a desired direction.	//
//////////////////////////////////////////////////////////
Coursework::Coursework(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input *in) : BaseApplication(hinstance, hwnd, screenWidth, screenHeight, in)
{

	// Setting the camera position to where we want.
	m_Camera->SetPosition(10.0f, 30.0f, -60.0f);

	// Setting the camera rotation to how we want to start looking at the scene.
	m_Camera->SetRotation(10.0f, 25.0f, 0.0f);

	// Initialising a new scene.
	m_scene = new Scene(hwnd, screenWidth, screenHeight, m_Direct3D, m_Input, m_Camera, m_Timer);

}

//////////////////////////////////////////////////////////
//======================================================//
//						Destructor						//
//======================================================//
// This will release all of our pointers.				//
//////////////////////////////////////////////////////////
Coursework::~Coursework()
{

	// Run base application deconstructor
	BaseApplication::~BaseApplication();

	// Release the scene objects.
	m_scene->~Scene();

}

//////////////////////////////////////////////////////////
//======================================================//
//							Frame						//
//======================================================//
// This will process what happens in our application	//
// every frame update.									//
//////////////////////////////////////////////////////////
bool Coursework::Frame()
{

	bool result;

	result = BaseApplication::Frame();
	if (!result)
	{
		return false;
	}

	// Render the graphics.
	result = Render();
	if (!result)
	{
		return false;
	}

	return true;

}

//////////////////////////////////////////////////////////
//======================================================//
//							Render						//
//======================================================//
// This will render everything in our scene.			//
//////////////////////////////////////////////////////////
bool Coursework::Render()
{

	// Render our scene.
	m_scene->RenderTheScene(m_Timer->GetTime());
	return true;

}