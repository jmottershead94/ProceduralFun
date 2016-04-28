// Jason Mottershead, 1300455.

// Master shadow shader class header file.
// This class will be responsible for multiple lighting and shadows.

// Header guard.
#ifndef _MASTERSHADOWSHADER_H_
#define _MASTERSHADOWSHADER_H_

// Includes.
#include <array>
#include "BaseShader.h"
#include "PointLight.h"

// Namespaces.
using namespace std;
using namespace DirectX;

// Master shadow shader IS A base shader, therefore inherits from it.
class MasterShadowShader : public BaseShader
{

	private:
		// Used for matrices and multiple lights.
		struct MatrixBufferType
		{
			XMMATRIX world;
			XMMATRIX view;
			XMMATRIX projection;
			XMMATRIX lightView[2];
			XMMATRIX lightProjection[2];
		};

		// Light colours.
		struct LightBufferType
		{
			XMFLOAT4 ambient[2];
			XMFLOAT4 diffuse[2];
		};

		// Light position.
		struct LightBufferType2
		{
			XMFLOAT3 position[2];
			float padding[2];
		};
		
	public:
		// Methods.
		MasterShadowShader(ID3D11Device* device, HWND hwnd);
		~MasterShadowShader();
		void SetShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX &world, const XMMATRIX &view, const XMMATRIX &projection, ID3D11ShaderResourceView* texture, ID3D11ShaderResourceView* depthMap[2], std::array<PointLight*, 2> light);
		void Render(ID3D11DeviceContext* deviceContext, int vertexCount);

	private:
		// Attributes.
		ID3D11Buffer* m_matrixBuffer;
		ID3D11SamplerState* m_sampleState;
		ID3D11SamplerState* m_sampleStateClamp;
		ID3D11Buffer* m_LightBuffer;
		ID3D11Buffer* m_LightBuffer2;

		// Methods.
		void InitShader(WCHAR*, WCHAR*);		

};

#endif