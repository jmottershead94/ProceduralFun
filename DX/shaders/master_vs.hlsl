// Jason Mottershead, 1300455.

// Master vertex shader file.
// This will pass along position, texture and normal values for the hull, domain and pixel shader.

// Vertex input.
struct InputType
{
    float4 position : POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
};

// Vertex output / hull input.
struct OutputType
{
	float4 position : POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};

OutputType main(InputType input)
{

	// Change the position vector to be 4 units for proper matrix calculations.
	input.position.w = 1.0f;

	// Pass the output along.
	return input;

}
