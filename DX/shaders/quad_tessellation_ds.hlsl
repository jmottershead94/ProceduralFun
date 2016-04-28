// Jason Mottershead, 1300455.

// Quad tessellation domain shader file.
// This will pass along position, texture and normal values for the domain and pixel shader.
// After tessellation the domain shader processes the all the vertices.
// Here we will apply our height and normal maps.
Texture2D heightMapTexture	: register(t1);		// The height map texture.
Texture2D normalMapTexture	: register(t2);		// The normap map texture.
SamplerState sampleType		: register(s0);		// The sample type.

// Matrix buffer.
cbuffer MatrixBuffer : register(b0)
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};

// Domain constant output.
struct ConstantOutputType
{
    float edges[4] : SV_TessFactor;
    float inside[2] : SV_InsideTessFactor;
};

// Domain input.
struct InputType
{
	float4 position : POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};

// Domain output / pixel shader input.
struct OutputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float3 position3D : TEXCOORD2;
};

[domain("quad")]
OutputType main(ConstantOutputType input, float2 uvwCoord : SV_DomainLocation, const OutputPatch<InputType, 4> patch)
{
	OutputType output;
	float3 vertexPosition = {0.0f, 0.0f, 0.0f};
	float2 texturePosition = { 0.0f, 0.0f };
	float3 normalPosition = { 0.0f, 0.0f, 0.0f };

	// Calculate the current position of the vertex.
	float3 v1 = lerp(patch[0].position, patch[1].position, (1 - uvwCoord.y));
	float3 v2 = lerp(patch[2].position, patch[3].position, (1 - uvwCoord.y));
	vertexPosition = lerp(v1, v2, uvwCoord.x);

	// Calculate the current texture of the vertex.
	float2 t1 = lerp(patch[0].tex, patch[1].tex, (1 - uvwCoord.y));
	float2 t2 = lerp(patch[2].tex, patch[3].tex, (1 - uvwCoord.y));
	texturePosition = lerp(t1, t2, uvwCoord.x);

	// Calculate the current normal of the vertex.
	float3 n1 = lerp(patch[0].normal, patch[1].normal, (1 - uvwCoord.y));
	float3 n2 = lerp(patch[2].normal, patch[3].normal, (1 - uvwCoord.y));
	normalPosition = lerp(n1, n2, uvwCoord.x);

	// Sample the current texture colour at a specified position.
	// This will displace vertices based on the height map texture.
	vertexPosition.y = (heightMapTexture.SampleLevel(sampleType, texturePosition, 1) * 32.0f);

	// Sample the current normal colour at a specified position.
	// This should alter normals based on the normal map provided.
	normalPosition.y = normalMapTexture.SampleLevel(sampleType, texturePosition, 1);

	// Calculate the position of the new vertex against the world, view, and projection matrices.
	output.position = mul(float4(vertexPosition, 1.0f), worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

	// Calculating the texture display based on the current texture position.
	output.tex = texturePosition;

	// Calculating the normals for the quad.
	output.normal = mul(normalPosition, (float3x3)worldMatrix);
	output.normal = normalize(output.normal);

	// Working out the 3D world position for lighting calculations.
	output.position3D = mul(vertexPosition, worldMatrix);

    return output;
}