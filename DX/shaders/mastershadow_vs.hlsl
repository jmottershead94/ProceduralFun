// Jason Mottershead, 1300455.

// Master shadow vertex shader file.
// This will pass along position, texture and normal values for the pixel shader.
// Here we will calculate light positions for multiple lights.
// These will be passed along to calculate shadows for multiple lights.

// Matrix buffer.
cbuffer MatrixBuffer : register(b0)
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
	matrix lightViewMatrix[2];
	matrix lightProjectionMatrix[2];
};

// Light buffer.
cbuffer LightBuffer2 : register(b1)
{
    float3 lightPosition[2];
	float padding[2];
};

// Vertex input.
struct InputType
{
    float4 position : POSITION;
    float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};

// Vertex output / pixel input.
struct OutputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
    float4 lightViewPosition : TEXCOORD1;
	float3 lightPos : TEXCOORD2;
	float4 lightViewPosition2 : TEXCOORD3;
	float3 lightPos2 : TEXCOORD4;
};

OutputType main(InputType input)
{
    OutputType output;
	float4 worldPosition;
        
	// Change the position vector to be 4 units for proper matrix calculations.
    input.position.w = 1.0f;

	// Calculate the position of the vertex against the world, view, and projection matrices.
    output.position = mul(input.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    
	// Calculate the position of the vertice as viewed by the light source.
    output.lightViewPosition = mul(input.position, worldMatrix);
    output.lightViewPosition = mul(output.lightViewPosition, lightViewMatrix[0]);
    output.lightViewPosition = mul(output.lightViewPosition, lightProjectionMatrix[0]);

	output.lightViewPosition2 = mul(input.position, worldMatrix);
	output.lightViewPosition2 = mul(output.lightViewPosition2, lightViewMatrix[1]);
	output.lightViewPosition2 = mul(output.lightViewPosition2, lightProjectionMatrix[1]);

	// Store the texture coordinates for the pixel shader.
    output.tex = input.tex;
    
	// Calculate the normal vector against the world matrix only.
    output.normal = mul(input.normal, (float3x3)worldMatrix);
	
    // Normalize the normal vector.
    output.normal = normalize(output.normal);

    // Calculate the position of the vertex in the world.
    worldPosition = mul(input.position, worldMatrix);

    // Determine the light position based on the position of the light and the position of the vertex in the world.
    output.lightPos = lightPosition[0].xyz - worldPosition.xyz;

    // Normalize the light position vector.
    output.lightPos = normalize(output.lightPos);

	// Determine the light position based on the position of the light and the position of the vertex in the world.
	output.lightPos2 = lightPosition[1].xyz - worldPosition.xyz;

	// Normalize the light position vector.
	output.lightPos2 = normalize(output.lightPos);

	return output;
}