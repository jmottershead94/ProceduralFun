// Light shader.h
// Basic single light shader setup
#ifndef _MANIPULATIONSHADER_H_
#define _MANIPULATIONSHADER_H_

#include "BaseShader.h"
#include "light.h"
#include "Timer.h"

using namespace std;
using namespace DirectX;

class ManipulationShader : public BaseShader
{
private:
	struct LightBufferType
	{
		XMFLOAT4 ambient;
		XMFLOAT4 diffuse;
		XMFLOAT3 direction;
		float padding;
	};

	struct TimeBufferType
	{
		float frequency;
		float height;
		float time;
		float width;
	};

public:

	ManipulationShader(ID3D11Device* device, HWND hwnd);
	~ManipulationShader();

	void SetShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX &world, const XMMATRIX &view, const XMMATRIX &projection, ID3D11ShaderResourceView* texture, Light* light, Timer* timer, float& frequency, float& height, float& width);
	void Render(ID3D11DeviceContext* deviceContext, int vertexCount);

private:
	void InitShader(WCHAR*, WCHAR*);

private:
	float dt_;
	ID3D11Buffer* m_matrixBuffer;
	ID3D11SamplerState* m_sampleState;
	ID3D11Buffer* m_lightBuffer;
	ID3D11Buffer* m_timeBuffer;
};

#endif