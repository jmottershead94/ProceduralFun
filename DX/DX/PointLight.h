// Jason Mottershead, 1300455.

// Point light class header file.
// This class will set up a point light with its' own sphere mesh.
// This will allow us to easily locate our point light.

// Header guard.
#ifndef _POINTLIGHT_H_
#define _POINTLIGHT_H_

// Includes.
#include "Light.h"
#include "SphereMesh.h"

class PointLight
{

	public:
		// Methods.
		PointLight(ID3D11Device* device, WCHAR* textureFilename, XMFLOAT3 diffuseColour, XMFLOAT3 desiredOrigin, XMFLOAT3 lookAtPoint);
		~PointLight();

		// Getters.
		// This will return the sphere for this point light.
		inline SphereMesh* GetSphereMesh()	{ return m_pointlightSphere; }

		// This will return the position for this point light.
		inline XMFLOAT3& GetPosition()		{ return m_position; }

		// This function will return the light for this point light.
		inline Light* GetPointLight()		{ return m_light; };

	private:
		// Attributes.
		SphereMesh* m_pointlightSphere;		// The sphere for our point light.
		XMFLOAT3 m_position;				// The position of our point light mesh.
		Light* m_light;						// The point light itself.

};

#endif