// Jason Mottershead, 1300455.

// Coursework class header file.
// This class will set the scene for my coursework application.

// Header guard.
#ifndef _COURSEWORK_H
#define _COURSEWORK_H

// Includes.
#include "baseapplication.h"
#include "Scene.h"

// Coursework IS A base application, therefore inherits from it.
class Coursework : public BaseApplication
{

	public:
		// Methods.
		Coursework(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input* in);
		~Coursework();
		bool Frame();

	private:
		// Attributes.
		Scene* m_scene;		// Access to the scene we want.

		// Methods.
		bool Render();

};

#endif