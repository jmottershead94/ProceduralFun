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

float CosineInterpolation(float x, float y, float fractional)
{
	float ft = 3.141529f * fractional;
	float f = (1.0f - cos(ft)) * 0.5f;

	//return ((3 * pow(f, 2)) - (2 * pow(f, 3)));
	return x * (1.0f - f) + y * f;
}

float Noise(float2 xy)
{
	float2 noise = (frac(sin(dot(xy, float2(12.9898f, 78.233f) * 2.0f)) * 43758.5453f));
	return (abs(noise.x + noise.y) * 0.5f);
}

float SmoothNoise(float integer_x, float integer_y)
{
	float corners = (Noise(float2(integer_x - 1, integer_y - 1)) + Noise(float2(integer_x + 1, integer_y + 1)) + Noise(float2(integer_x + 1, integer_y - 1)) + Noise(float2(integer_x - 1, integer_y + 1))) / 16.0f;
	float sides = (Noise(float2(integer_x, integer_y - 1)) + Noise(float2(integer_x, integer_y + 1)) + Noise(float2(integer_x + 1, integer_y)) + Noise(float2(integer_x - 1, integer_y))) / 8.0f;
	float center = Noise(float2(integer_x, integer_y)) / 4.0f;

	return corners + sides + center;
}

float InterpolatedNoise(float x, float y)
{
	float integer_x = x - frac(x), fractional_x = frac(x);
	float integer_y = y - frac(y), fractional_y = frac(y);

	float p1 = SmoothNoise(integer_x, integer_y);
	float p2 = SmoothNoise(integer_x + 1, integer_y);
	float p3 = SmoothNoise(integer_x, integer_y + 1);
	float p4 = SmoothNoise(integer_x + 1, integer_y + 1);

	p1 = CosineInterpolation(p1, p2, fractional_x);
	p2 = CosineInterpolation(p3, p4, fractional_x);

	return CosineInterpolation(p1, p2, fractional_y);
}

float CreatePerlinNoise(int octaves, float x, float y)
{
	float result = 0.0f;		// The total value so far.
	float amplitude = 1.0f;		// Accumulates highest amplitude.
	float frequency = 0.0f;
	float persistance = 0.1f;
	float maxAmplitude = 0.0f;

	for (int i = 0; i <= octaves; i++)
	{
		// Get the noise sample.
		result += InterpolatedNoise(x * frequency, y * frequency) * amplitude;

		// Make the frequency length twice as small.
		frequency += 2.0f;

		// Add to our maximum possible amplitude.
		maxAmplitude += amplitude;

		// Reduce the amplitude according to persistance from the next octave.
		amplitude *= persistance;
	}

	return (result / maxAmplitude);
}

float CreateRigidNoise(int octaves, float x, float y)
{
	float result = 0.0f;		// The total value so far.
	float amplitude = 1.0f;		// Accumulates highest amplitude.
	float frequency = 0.0f;
	float persistance = 0.1f;
	float maxAmplitude = 0.0f;

	for (int i = 0; i <= octaves; i++)
	{
		// Get the noise sample.
		result += ((1.0f - abs(InterpolatedNoise(x * frequency, y * frequency))) * 2.0f - 1.0f) * amplitude;

		// Make the frequency length twice as small.
		frequency += 2.0f;

		// Add to our maximum possible amplitude.
		maxAmplitude += amplitude;

		// Reduce the amplitude according to persistance from the next octave.
		amplitude *= persistance;
	}

	return (result / maxAmplitude);
}

float4 main(InputType input) : SV_TARGET
{
	// Setting up the noise values.
	float4 textureColour = { 0.0f, 0.0f, 0.0f, 1.0f };
	float newX = (input.tex.x * 50.0f) + time;							// Processing the x texture coordinate of the texture.
	float newY = (input.tex.y * 50.0f) + time;							// Processing the y texture coordinate.
	float perlinNoise = CreatePerlinNoise(6, newX, newY);				// Creating a perlin noise sample.
	float rigidNoise = (CreateRigidNoise(6, newX, newY) * 0.015f);		// Creating some rigid noise.

	// Setting up the pulsing storms.
	float constantValue = CreateRigidNoise(3, time, time);
	float t1 = InterpolatedNoise(input.tex.x * 2.0f, input.tex.y * 2.0f) * constantValue;
	float t2 = InterpolatedNoise(input.tex.x + 800.0f, input.tex.y + 800.0f) * constantValue;
	float t3 = InterpolatedNoise(input.tex.x + 1600.0f, input.tex.y + 1600.0f) * constantValue;
	float threshold = max(t1 * t2 * t3, 0.0f) * 50.0f;
	
	// Applying the final noise layer.
	float overallNoise = (CreatePerlinNoise(1, (input.tex.x * 0.1f), (input.tex.y * 0.1f)) * threshold);
	
	// Add all of our noise values together.
	float index = perlinNoise + rigidNoise + overallNoise;
	
	// Sample our current texture.
	textureColour = shaderTexture.Sample(SampleType, index);

	// Return the current texture colour.
	return textureColour;
}