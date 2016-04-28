// Jason Mottershead, 1300455.

// Particle shader class header file.
// This class will be responsible for creating geometry and using the geometry shader.

// Header guard.
#ifndef _PARTICLESHADER_H_
#define _PARTICLESHADER_H_

// Includes.
#include "BaseShader.h"

// Namespaces.
using namespace std;
using namespace DirectX;

// Particle shader IS A base shader, therefore inherits from it.
class ParticleShader : public BaseShader
{

	public:
		// Methods.
		ParticleShader(ID3D11Device* device, HWND hwnd);
		~ParticleShader();
		void SetShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX &world, const XMMATRIX &view, const XMMATRIX &projection, ID3D11ShaderResourceView* texture, XMFLOAT3 translate);
		void Render(ID3D11DeviceContext* deviceContext, int vertexCount);

	private:
		// Attributes.
		ID3D11Buffer* m_matrixBuffer;
		ID3D11SamplerState* m_sampleState;
		ID3D11Buffer* m_positionBuffer;

		// Methods.
		void InitShader(WCHAR* vsFilename, WCHAR* psFilename);
		void InitShader(WCHAR* vsFilename, WCHAR* gsFilename, WCHAR* psFilename);

		// Structs.
		struct PositionBufferType
		{
			static XMFLOAT3 gPositions[4];
			XMFLOAT3 translation;
			float padding;
			XMFLOAT4 positionPadding;
		};
	
};

#endif