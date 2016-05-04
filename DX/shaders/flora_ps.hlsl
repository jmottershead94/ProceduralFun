// Jason Mottershead, 1300455.

Texture2D shaderTexture	: register(t0);		// Register our shader texture, in this case, our terrain texture.
SamplerState SampleType	: register(s0);		// The sample type.

// Pixel shader input.
struct InputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};

float4 main(InputType input) : SV_TARGET
{
	float4 textureColor, finalColour, invertedColour;

	// Sample the pixel color from the texture using the sampler at this texture coordinate location.
	textureColor = shaderTexture.Sample(SampleType, input.tex);

	return textureColor;
}