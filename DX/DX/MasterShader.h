// Jason Mottershead, 1300455.

// Master shader class header file.
// This class will be responsible for lighting, tessellation, height and normal mapping.

// Header guard.
#ifndef _MASTERSHADER_H_
#define _MASTERSHADER_H_

// Includes.
#include "BaseShader.h"
#include "PointLightGroup.h"

// Namespaces.
using namespace std;
using namespace DirectX;

// Master shader IS A base shader, therefore inherits from it.
class MasterShader : public BaseShader
{

	private:
		// Used for tessellation.
		struct TessellationBufferType
		{
			float tessellationFactor;
			XMFLOAT3 padding;
		};

		// Used for lighting.
		struct LightBufferType
		{
			XMFLOAT4 ambient[3];
			XMFLOAT4 diffuse[3];
			XMFLOAT4 position[3];
		};

	public:
		// Methods.
		MasterShader(ID3D11Device* device, HWND hwnd);
		~MasterShader();
		void SetShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX &world, const XMMATRIX &view, const XMMATRIX &projection, ID3D11ShaderResourceView* texture, ID3D11ShaderResourceView* heightMapTexture, ID3D11ShaderResourceView* normalMapTexture, float tessellationFactor, PointLightGroup* light);
		void Render(ID3D11DeviceContext* deviceContext, int vertexCount);

	private:
		// Attributes.
		ID3D11Buffer* m_matrixBuffer;
		ID3D11SamplerState* m_sampleState;
		ID3D11Buffer* m_tessellationBuffer;
		ID3D11Buffer* m_lightBuffer;

		// Methods.
		void InitShader(WCHAR* vsFilename, WCHAR* psFilename);
		void InitShader(WCHAR* vsFilename, WCHAR* hsFilename, WCHAR* dsFilename, WCHAR* psFilename);

};

#endif