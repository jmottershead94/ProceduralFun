#ifndef _NOISEPLANE_H_
#define _NOISEPLANE_H_

#include "BaseMesh.h"
#include "PerlinNoise.h"

class NoisePlane : public BaseMesh
{

	public:
		// Methods.
		NoisePlane(ID3D11Device* device, WCHAR* textureFilename, int resolution = 100);
		~NoisePlane();
		

	protected:
		// Attributes.
		int m_resolution;
		PerlinNoise* m_perlinNoise;	// Perlin noise test.

		// Methods.
		void InitBuffers(ID3D11Device* device);
		void InitBuffersNoise(ID3D11Device* device);

};

#endif