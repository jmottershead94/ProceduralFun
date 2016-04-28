// Jason Mottershead, 1300455.

// Particle pixel shader file.
// This will output a colour based on our input.

// Pixel input.
struct InputType
{
	float4 position : SV_POSITION;
	float4 colour : COLOR;
};

float4 main(InputType input) : SV_TARGET
{

	return input.colour;

}