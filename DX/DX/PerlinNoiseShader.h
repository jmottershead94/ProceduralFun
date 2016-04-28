// texture shader.h
#ifndef _PERLINNOISESHADER_H_
#define _PERLINNOISESHADER_H_

#include "BaseShader.h"
#include "ObjectIDNumber.h"
#include "ProceduralIDNumber.h"
#include "Timer.h"

using namespace std;
using namespace DirectX;

class PerlinNoiseShader : public BaseShader
{

public:

	PerlinNoiseShader(ID3D11Device* device, HWND hwnd, int proceduralIDNumber);
	~PerlinNoiseShader();

	void SetShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX &world, const XMMATRIX &view, const XMMATRIX &projection, ID3D11ShaderResourceView* texture, Timer* timer);
	void Render(ID3D11DeviceContext* deviceContext, int vertexCount);

private:
	// Attributes.
	struct TimeBufferType
	{
		float time;
		XMFLOAT3 padding;
	};

	// Methods.
	void InitShader(WCHAR*, WCHAR*);

private:
	float dt;
	ID3D11Buffer* m_matrixBuffer;
	ID3D11SamplerState* m_sampleState;
	ID3D11Buffer* m_timeBuffer;
};

#endif