// Mesh.cpp
#include "particlemesh.h"

ParticleMesh::ParticleMesh(ID3D11Device* device, WCHAR* textureFilename) : PointMesh(device, textureFilename)
{
	// Initialize the vertex and index buffer that hold the geometry for the triangle.
	InitBuffers(device);

	// Load the texture for this model.
	LoadTexture(device, textureFilename);
}

ParticleMesh::~ParticleMesh()
{
	// Run parent deconstructor
	PointMesh::~PointMesh();
}

void ParticleMesh::Kill()
{

}

void ParticleMesh::Spawn(float dt)
{

}

void ParticleMesh::Update(float dt)
{
	
	Kill();

	Spawn(dt);

}