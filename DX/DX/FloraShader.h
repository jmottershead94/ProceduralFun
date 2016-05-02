// texture shader.h
#ifndef _FLORASHADER_H_
#define _FLORASHADER_H_

#include "BaseShader.h"
#include "ObjectIDNumber.h"
#include "ProceduralIDNumber.h"
#include "Timer.h"

using namespace std;
using namespace DirectX;

class FloraShader : public BaseShader
{

public:

	FloraShader(ID3D11Device* device, HWND hwnd, int proceduralIDNumber);
	~FloraShader();

	void SetShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX &world, const XMMATRIX &view, const XMMATRIX &projection, ID3D11ShaderResourceView* texture, float gravity, int ID);
	void Render(ID3D11DeviceContext* deviceContext, int vertexCount);

private:
	// Attributes.
	struct GravityBufferType
	{
		float gravity;
		float ID;
		XMFLOAT2 padding;
	};

	// Methods.
	void InitShader(WCHAR*, WCHAR*);

private:
	float dt;
	ID3D11Buffer* m_matrixBuffer;
	ID3D11SamplerState* m_sampleState;
	ID3D11Buffer* m_gravityBuffer;
};

#endif