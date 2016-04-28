// Jason Mottershead, 1300455.

// Point light group class header file.
// This class will only have the purpose of grouping three of my point lights together.

// Header guard.
#ifndef _POINTLIGHTGROUP_H_
#define _POINTLIGHTGROUP_H_

// Includes.
#include <array>
#include "SphereMesh.h"
#include "PointLight.h"

class PointLightGroup
{

	public:
		// Methods.
		PointLightGroup(ID3D11Device* device, WCHAR* textureFilename, XMFLOAT3 desiredOrigin);
		~PointLightGroup();
		
		// Getters.
		// This will return our array of point lights.
		inline std::array<PointLight*, 3>& GetPointLights() { return m_pointLights; }

	private:
		// Attributes.
		std::array<PointLight*, 3> m_pointLights;

};

#endif