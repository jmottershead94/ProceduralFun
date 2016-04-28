struct InputType
{
    float4 position : SV_POSITION;
    float4 depthPosition : TEXCOORD0;
};

float4 main(InputType input) : SV_TARGET
{
	float depthValue = 0.0f;
	float4 color = {0.0f, 0.0f, 0.0f, 0.0f};

	// Get the depth value of the pixel by dividing the Z pixel depth by the homogeneous W coordinate.
	depthValue = input.depthPosition.z / input.depthPosition.w;
	
	color = float4(depthValue, depthValue, depthValue, 1.0f);

	// Showing off different depth values.
	/*if (depthValue < 0.9f)
	{
		color = float4(1.0f, 0.0f, 0.0f, 1.0f);
	}
	else if (depthValue > 0.9f && depthValue < 0.925f)
	{
		color = float4(0.0f, 1.0f, 0.0f, 1.0f);
	}
	else if (depthValue > 0.925f)
	{
		color = float4(0.0f, 0.0f, 1.0f, 1.0f);
	}*/

	return color;
}