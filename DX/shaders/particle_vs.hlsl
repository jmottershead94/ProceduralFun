// Jason Mottershead, 1300455.

// Particle vertex shader file.
// This will pass along the vertex values and not change anything major.

// Vertex input.
struct InputType
{
    float4 position : POSITION;
	float4 colour : COLOR;
};

// Vertex output / geometry input.
struct OutputType
{
	float4 position : SV_POSITION;
	float4 colour : COLOR;
};

InputType main(InputType input)
{

	return input;

}