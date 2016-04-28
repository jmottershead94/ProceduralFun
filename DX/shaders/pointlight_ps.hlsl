// Light pixel shader
// Calculate ambient and diffuse lighting for a single light (also texturing)

Texture2D shaderTexture : register(t0);
SamplerState SampleType : register(s0);

cbuffer LightBuffer : register(b0)
{
	float4 ambientColour[3];
	float4 diffuseColour[3];
	float4 position[3];
};

struct InputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
	float3 position3D : TEXCOORD2;
};

float4 main(InputType input) : SV_TARGET
{
    float4 textureColour;
    float3 lightDir;
    float lightIntensity;
	float4 colour[3];
	float4 finalColour;

    // Sample the pixel color from the texture using the sampler at this texture coordinate location.
    textureColour = shaderTexture.Sample(SampleType, input.tex);

	for (int i = 0; i < 3; i++)
	{
		// Setting a default amount of colour.
		colour[i] = ambientColour[0];

		// Invert the light direction for calculations.
		lightDir = normalize(input.position3D - position[i]);

		// Calculate the amount of light on this pixel.
		lightIntensity = saturate(dot(input.normal, -lightDir));

		// If the light intensity is greater than 0.
		if (lightIntensity > 0.0f)
		{
			// Determine the final amount of diffuse color based on the diffuse color combined with the light intensity.
			// Same calculation as before.
			colour[i] += saturate(diffuseColour[i] * lightIntensity);

			colour[i] = saturate(colour[i]);
		}

		// Multiply the texture pixel and the final diffuse color to get the final pixel color result.
		colour[i] = colour[i] * textureColour;
	}

	finalColour = colour[0] + colour[1] + colour[2];
	finalColour = saturate(finalColour);

	return finalColour;
}

