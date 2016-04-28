// particle shader.cpp
#include "particleshader.h"

//////////////////////////////////////////////////////////
//======================================================//
//						Constructor						//
//======================================================//
// This will be used for initialising our shader files.	//
//////////////////////////////////////////////////////////
ParticleShader::ParticleShader(ID3D11Device* device, HWND hwnd) : BaseShader(device, hwnd)
{

	// Load our shader files.
	InitShader(L"../shaders/particle_vs.hlsl", L"../shaders/particle_gs.hlsl", L"../shaders/particle_ps.hlsl");

}

//////////////////////////////////////////////////////////
//======================================================//
//						Destructor						//
//======================================================//
// This will release all of our pointers.				//
//////////////////////////////////////////////////////////
ParticleShader::~ParticleShader()
{

	// Release the sampler state.
	if (m_sampleState)
	{
		m_sampleState->Release();
		m_sampleState = 0;
	}

	// Release the matrix constant buffer.
	if (m_matrixBuffer)
	{
		m_matrixBuffer->Release();
		m_matrixBuffer = 0;
	}

	// Release the layout.
	if (m_layout)
	{
		m_layout->Release();
		m_layout = 0;
	}

	//Release base shader components
	BaseShader::~BaseShader();

}

//////////////////////////////////////////////////////////
//======================================================//
//						InitShader						//
//======================================================//
// This will set up our buffer descriptions and will	//
// load in our shader files through the base shader		//
// class.												//
//////////////////////////////////////////////////////////
void ParticleShader::InitShader(WCHAR* vsFilename, WCHAR* psFilename)
{

	D3D11_BUFFER_DESC matrixBufferDesc;
	D3D11_SAMPLER_DESC samplerDesc;
	D3D11_BUFFER_DESC tessellationBufferDesc;
	D3D11_BUFFER_DESC positionBufferDesc;

	// Load (+ compile) shader files
	loadVertexShader(vsFilename);
	loadPixelShader(psFilename);

	// Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	m_device->CreateBuffer(&matrixBufferDesc, NULL, &m_matrixBuffer);

	// Create a texture sampler state description.
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_MIRROR;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_MIRROR;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_MIRROR;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// Create the texture sampler state.
	m_device->CreateSamplerState(&samplerDesc, &m_sampleState);

	// Setup position buffer
	// Setup the description of the light dynamic constant buffer that is in the pixel shader.
	// Note that ByteWidth always needs to be a multiple of 16 if using D3D11_BIND_CONSTANT_BUFFER or CreateBuffer will fail.
	positionBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	positionBufferDesc.ByteWidth = sizeof(PositionBufferType);
	positionBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	positionBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	positionBufferDesc.MiscFlags = 0;
	positionBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the geometry shader constant buffer from within this class.
	m_device->CreateBuffer(&positionBufferDesc, NULL, &m_positionBuffer);

}

//////////////////////////////////////////////////////////
//======================================================//
//						InitShader						//
//======================================================//
// This will call the above function, but will also		//
// load in additional shader files for geometry.		//
//////////////////////////////////////////////////////////
void ParticleShader::InitShader(WCHAR* vsFilename, WCHAR* gsFilename, WCHAR* psFilename)
{

	// InitShader must be overwritten and it will load both vertex and pixel shaders + setup buffers
	InitShader(vsFilename, psFilename);

	// Load other required shaders.
	loadGeometryShader(gsFilename);

}

//////////////////////////////////////////////////////////
//======================================================//
//					SetShaderParameters					//
//======================================================//
// This will set the parameters for our buffers used in	//
// the shader files, as well as allocate shader			//
// resources.											//
//////////////////////////////////////////////////////////
void ParticleShader::SetShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX &worldMatrix, const XMMATRIX &viewMatrix, const XMMATRIX &projectionMatrix, ID3D11ShaderResourceView* texture, XMFLOAT3 translate)
{

	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType* dataPtr;
	unsigned int bufferNumber;
	XMMATRIX tworld, tview, tproj;
	PositionBufferType* positionPtr;

	// Transpose the matrices to prepare them for the shader.
	tworld = XMMatrixTranspose(worldMatrix);
	tview = XMMatrixTranspose(viewMatrix);
	tproj = XMMatrixTranspose(projectionMatrix);

	// Lock the constant buffer so it can be written to.
	result = deviceContext->Map(m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

	// Get a pointer to the data in the constant buffer.
	dataPtr = (MatrixBufferType*)mappedResource.pData;

	// Copy the matrices into the constant buffer.
	dataPtr->world = tworld;// worldMatrix;
	dataPtr->view = tview;
	dataPtr->projection = tproj;

	// Unlock the constant buffer.
	deviceContext->Unmap(m_matrixBuffer, 0);

	// Set the position of the constant buffer in the vertex shader.
	bufferNumber = 0;

	// Now set the constant buffer in the geometry shader with the updated values.
	deviceContext->GSSetConstantBuffers(bufferNumber, 1, &m_matrixBuffer);

	// Setting up the time pointer values.
	deviceContext->Map(m_positionBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	positionPtr = (PositionBufferType*)mappedResource.pData;
	positionPtr->translation = { translate.x, translate.y, translate.z };
	positionPtr->padding = 0.0f;
	positionPtr->positionPadding = { 0.0f, 0.0f, 0.0f, 0.0f };

	// Unlock the buffer.
	deviceContext->Unmap(m_positionBuffer, 0);

	// Set the buffer number.
	bufferNumber = 1;

	// Now set the constant buffer in the geometry shader with the updated values.
	deviceContext->GSSetConstantBuffers(bufferNumber, 1, &m_positionBuffer);

}

//////////////////////////////////////////////////////////
//======================================================//
//							Render						//
//======================================================//
// This will render anything that uses this shader.		//
//////////////////////////////////////////////////////////
void ParticleShader::Render(ID3D11DeviceContext* deviceContext, int indexCount)
{

	// Set the sampler state in the pixel shader.
	deviceContext->PSSetSamplers(0, 1, &m_sampleState);

	// Base render function.
	BaseShader::Render(deviceContext, indexCount);

}