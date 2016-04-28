// Jason Mottershead, 1300455.

// Master shadow pixel shader file.
// This will output a colour based on the lighting calculations.
// Here we will calculate the final colour for multiple shadows and multiple lights.

Texture2D shaderTexture : register(t0);			// The shader texture, in this case either the brick teapot, or the plane.
Texture2D depthMapTexture : register(t1);		// The depth map for the first shadow.
Texture2D depthMapTexture2 : register(t2);		// The depth map for the second shadow.

SamplerState SampleTypeWrap  : register(s0);	// The sample type wrap.
SamplerState SampleTypeClamp : register(s1);	// The sample type clamp.

// Light buffer.
cbuffer LightBuffer : register(b0)
{
	float4 ambientColor[2];
	float4 diffuseColor[2];
};

// Pixel input.
struct InputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
    float4 lightViewPosition : TEXCOORD1;
	float3 lightPos : TEXCOORD2;
	float4 lightViewPosition2 : TEXCOORD3;
	float3 lightPos2 : TEXCOORD4;
};

float4 main(InputType input) : SV_TARGET
{
	float bias;
    float4 color;
	float2 projectTexCoord;
	float depthValue;
	float lightDepthValue;
    float lightIntensity;
	float4 textureColor;
	
	// Set the bias value for fixing the floating point precision issues.
	bias = 0.0001f;

	// Set the default output color to the ambient light value for all pixels.
    color = ambientColor[0];

	// Calculate the projected texture coordinates.
	projectTexCoord.x =  input.lightViewPosition.x / input.lightViewPosition.w / 2.0f + 0.5f;
	projectTexCoord.y = -input.lightViewPosition.y / input.lightViewPosition.w / 2.0f + 0.5f;

	// Determine if the projected coordinates are in the 0 to 1 range.  If so then this pixel is in the view of the light.
	if((saturate(projectTexCoord.x) == projectTexCoord.x) && (saturate(projectTexCoord.y) == projectTexCoord.y))
	{
		// Sample the shadow map depth value from the depth texture using the sampler at the projected texture coordinate location.
		depthValue = depthMapTexture.Sample(SampleTypeClamp, projectTexCoord).r;

		// Calculate the depth of the light.
		lightDepthValue = input.lightViewPosition.z / input.lightViewPosition.w;

		// Subtract the bias from the lightDepthValue.
		lightDepthValue = lightDepthValue - bias;

		// Compare the depth of the shadow map value and the depth of the light to determine whether to shadow or to light this pixel.
		// If the light is in front of the object then light the pixel, if not then shadow this pixel since an object (occluder) is casting a shadow on it.
		if(lightDepthValue < depthValue)
		{
		    // Calculate the amount of light on this pixel.
			lightIntensity = saturate(dot(input.normal, input.lightPos));

		    if(lightIntensity > 0.0f)
			{
				// Determine the final diffuse color based on the diffuse color and the amount of light intensity.
				color += (diffuseColor[0] * lightIntensity);
			}
		}
	}

	// Calculate the projected texture coordinates.
	projectTexCoord.x = input.lightViewPosition2.x / input.lightViewPosition2.w / 2.0f + 0.5f;
	projectTexCoord.y = -input.lightViewPosition2.y / input.lightViewPosition2.w / 2.0f + 0.5f;

	// Determine if the projected coordinates are in the 0 to 1 range.  If so then this pixel is in the view of the light.
	if ((saturate(projectTexCoord.x) == projectTexCoord.x) && (saturate(projectTexCoord.y) == projectTexCoord.y))
	{
		// Sample the shadow map depth value from the depth texture using the sampler at the projected texture coordinate location.
		depthValue = depthMapTexture2.Sample(SampleTypeClamp, projectTexCoord).r;

		// Calculate the depth of the light.
		lightDepthValue = input.lightViewPosition2.z / input.lightViewPosition2.w;

		// Subtract the bias from the lightDepthValue.
		lightDepthValue = lightDepthValue - bias;

		// Compare the depth of the shadow map value and the depth of the light to determine whether to shadow or to light this pixel.
		// If the light is in front of the object then light the pixel, if not then shadow this pixel since an object (occluder) is casting a shadow on it.
		if (lightDepthValue < depthValue)
		{
			// Calculate the amount of light on this pixel.
			lightIntensity = saturate(dot(input.normal, input.lightPos2));

			if (lightIntensity > 0.0f)
			{
				// Determine the final diffuse color based on the diffuse color and the amount of light intensity.
				color += (diffuseColor[1] * lightIntensity);
			}
		}
	}

	color = saturate(color);

	// Sample the pixel color from the texture using the sampler at this texture coordinate location.
	textureColor = shaderTexture.Sample(SampleTypeWrap, input.tex);

	// Combine the light and texture color.
	color = color * textureColor;

    return color;
}