// Point light group.cpp
#include "PointLightGroup.h"

//////////////////////////////////////////////////////////
//======================================================//
//						Constructor						//
//======================================================//
// This will set up our group of 3 point lights.		//
//////////////////////////////////////////////////////////
PointLightGroup::PointLightGroup(ID3D11Device* device, WCHAR* textureFilename, XMFLOAT3 desiredOrigin)
{

	// Setting up a group of 3 point lights.
	m_pointLights[0] = new PointLight(device, textureFilename, XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT3((desiredOrigin.x * 0.5f), desiredOrigin.y, (desiredOrigin.z)), XMFLOAT3(0.0f, 0.0f, 0.0f));
	m_pointLights[1] = new PointLight(device, textureFilename, XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT3((desiredOrigin.x + (desiredOrigin.x * 0.25f)), desiredOrigin.y, (desiredOrigin.z)), XMFLOAT3(0.0f, 0.0f, 0.0f));
	m_pointLights[2] = new PointLight(device, textureFilename, XMFLOAT3(1.0f, 1.0f, 0.0f), XMFLOAT3(desiredOrigin.x, desiredOrigin.y, (desiredOrigin.z + (desiredOrigin.z * 0.5f))), XMFLOAT3(0.0f, 0.0f, 0.0f));

}

//////////////////////////////////////////////////////////
//======================================================//
//						Destructor						//
//======================================================//
// This will release all of our pointers.				//
//////////////////////////////////////////////////////////
PointLightGroup::~PointLightGroup()
{

	// Releasing our pointers.
	if (!m_pointLights.empty())
	{
		for (auto& pointlight : m_pointLights)
		{
			delete pointlight;
			pointlight = nullptr;
		}
	}

}