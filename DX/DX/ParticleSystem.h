// Jason Mottershead, 1300455.

// Particle system class header file.
// This will give us access to a set of particles.

// Header guard.
#ifndef _PARTICLESYSTEM_H_
#define _PARTICLESYSTEM_H_

// Includes.
#include <vector>
#include "PointMesh.h"

// Namespaces.
using namespace DirectX;

// Particle system IS A base mesh, therefore inherits from it.
class ParticleSystem : public BaseMesh
{

	public:
		// Methods.
		ParticleSystem(ID3D11Device* device);
		~ParticleSystem();
		void SendData(ID3D11DeviceContext*);

		// Getters.
		// This will return a plain point mesh.
		inline PointMesh* NewPointMesh(ID3D11Device* device)	{ return new PointMesh(device, NULL); }

		// This will return our vector of particles.
		inline std::vector<PointMesh*>& GetParticles()			{ return m_particles; }

	protected:
		// Methods.
		void InitBuffers(ID3D11Device* device);		

	private:
		// Attributes.
		std::vector<PointMesh*> m_particles;					// Our vector of point particles, these will be transformed into quads later.
		const unsigned int m_numberOfParticles = 5;				// The number of particles we would like to start off with.

};

#endif