// Particle system.cpp
#include "particlesystem.h"

//////////////////////////////////////////////////////////
//======================================================//
//						Constructor						//
//======================================================//
// This will initialise our vertex and index buffers,	//
// as push some point meshes into our particles vector.	//
//////////////////////////////////////////////////////////
ParticleSystem::ParticleSystem(ID3D11Device* device)
{

	// Initialize the vertex and index buffer that hold the geometry for the triangle.
	InitBuffers(device);

	// We want X amount of particles.
	for (int i = 0; i < m_numberOfParticles; i++)
	{
		// Place a new particle into the vector.
		m_particles.push_back(NewPointMesh(device));
	}

}

//////////////////////////////////////////////////////////
//======================================================//
//						Destructor						//
//======================================================//
// This will call the parent destructor, and release	//
// any pointers in this class.							//
//////////////////////////////////////////////////////////
ParticleSystem::~ParticleSystem()
{

	// Run parent deconstructor
	BaseMesh::~BaseMesh();

	// Release the particles.
	// If we are some particles.
	if (!m_particles.empty())
	{
		// Empty the particles vector.
		m_particles.clear();
	}

}

//////////////////////////////////////////////////////////
//======================================================//
//						InitBuffers						//
//======================================================//
// This will initialise the buffers for each one of our	//
// particles.											//
//////////////////////////////////////////////////////////
void ParticleSystem::InitBuffers(ID3D11Device* device)
{

	// Iterating through our particles.
	for (auto& particle : m_particles)
	{
		// Initialise the buffers for each particle.
		particle->InitBuffers(device);
	}

}

//////////////////////////////////////////////////////////
//======================================================//
//						SendData						//
//======================================================//
// This will send render data for each particle.		//
//////////////////////////////////////////////////////////
void ParticleSystem::SendData(ID3D11DeviceContext* deviceContext)
{

	// Iterating through our particles.
	for (auto& particle : m_particles)
	{
		// Send data for each of our particles for rendering.
		particle->SendData(deviceContext);
	}

}