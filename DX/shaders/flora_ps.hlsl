// Jason Mottershead, 1300455.

Texture2D shaderTexture	: register(t0);		// Register our shader texture, in this case, our terrain texture.
SamplerState SampleType	: register(s0);		// The sample type.

cbuffer TimeBuffer : register(b0)
{
	float time;
	float3 padding;
};

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
	float4 red = { 1.0f, 0.0f, 0.0f, 1.0f };

	// Sample the pixel color from the texture using the sampler at this texture coordinate location.
	textureColor = texture0.Sample(Sampler0, input.tex);

	return textureColor;
	//return finalColour;
}