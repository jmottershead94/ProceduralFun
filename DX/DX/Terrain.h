// Jason Mottershead, 1300455.

// Terrain class header file.
// This class will help to make a terrain for our level.
// This will be used in combination with height maps.
// Adapted from the quad tessellation mesh class.

// Header guard.
#ifndef _TERRAIN_H_
#define _TERRAIN_H_

// Includes.
#include "BaseMesh.h"
#include "Texture.h"

// Terrain IS A base mesh, therefore inherits from it.
class Terrain : public BaseMesh
{

	public:
		// Methods.
		Terrain(ID3D11Device* device, HWND hwnd, WCHAR* textureFileName, WCHAR* heightMapFileName, WCHAR* normalMapFileName, int resolution = 100);
		~Terrain();
		void SendData(ID3D11DeviceContext*);

		// Getters.
		// This will return the current height map texture for displacement mapping.
		ID3D11ShaderResourceView* GetHeightMap() { return m_heightMap->GetTexture(); }

		// This will return the current normal map for the current height map for bump mapping.
		ID3D11ShaderResourceView* GetNormalMap() { return m_normalMap->GetTexture(); }

	protected:
		// Methods.
		void InitBuffers(ID3D11Device* device);

	private:
		// Attributes.
		int m_resolution;						// The resolution of our terrain - the dimensions. Specified above as 100 x 100.
		Texture* m_heightMap;					// The texture for our height map, this will allow us to displace vertices based on the texture.
		Texture* m_normalMap;					// The texture for our normal map, this will allow us to calculate normal values based on the texture.

};

#endif