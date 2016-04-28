// Point light.cpp
#include "PointLight.h"

//////////////////////////////////////////////////////////
//======================================================//
//						Constructor						//
//======================================================//
// This will initialise our point light sphere, the		//
// light pointer, and our position.						//
//////////////////////////////////////////////////////////
PointLight::PointLight(ID3D11Device* device, WCHAR* textureFilename, XMFLOAT3 diffuseColour, XMFLOAT3 desiredOrigin, XMFLOAT3 lookAtPoint)
{

	// Setting up the sphere mesh for the point light.
	m_pointlightSphere = new SphereMesh(device, textureFilename, 20);
	
	// Setting up the lighting.
	m_light = new Light();
	m_light->SetAmbientColour(0.1f, 0.1f, 0.1f, 1.0f);
	m_light->SetDiffuseColour(diffuseColour.x, diffuseColour.y, diffuseColour.z, 1.0f);
	m_light->SetPosition(desiredOrigin.x, desiredOrigin.y, desiredOrigin.z);
	m_light->SetLookAt(lookAtPoint.x, lookAtPoint.y, lookAtPoint.z);

	// Storing our position for easy access later.
	m_position = desiredOrigin;

}

//////////////////////////////////////////////////////////
//======================================================//
//						Destructor						//
//======================================================//
// This will release all of our pointers.				//
//////////////////////////////////////////////////////////
PointLight::~PointLight()
{

	// Release all of our pointers.
	if (m_pointlightSphere)
	{
		delete m_pointlightSphere;
		m_pointlightSphere = nullptr;
	}

	if (m_light)
	{
		delete m_light;
		m_light = nullptr;
	}

}