// texture shader.h
#ifndef _PERLINNOISESHADER_H_
#define _PERLINNOISESHADER_H_

#include "BaseShader.h"

using namespace std;
using namespace DirectX;

class PerlinNoiseShader : public BaseShader
{

public:

	PerlinNoiseShader(ID3D11Device* device, HWND hwnd);
	~PerlinNoiseShader();

	void SetShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX &world, const XMMATRIX &view, const XMMATRIX &projection, ID3D11ShaderResourceView* texture);
	void Render(ID3D11DeviceContext* deviceContext, int vertexCount);

private:
	void InitShader(WCHAR*, WCHAR*);

private:
	ID3D11Buffer* m_matrixBuffer;
	ID3D11SamplerState* m_sampleState;
};

#endif