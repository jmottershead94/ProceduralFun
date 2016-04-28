// Tessellation Mesh, point list for tessellation
#ifndef _PARTICLEMESH_H_
#define _PARTICLEMESH_H_

#include "PointMesh.h"

using namespace DirectX;

class ParticleMesh : public PointMesh
{

	public:
		// Methods.
		ParticleMesh(ID3D11Device* device, WCHAR* textureFilename);
		~ParticleMesh();
		void Kill();
		void Spawn(float dt);
		void Update(float dt);


};

#endif