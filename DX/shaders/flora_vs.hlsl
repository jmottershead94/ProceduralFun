// Jason Mottershead, 1300455.

// Perlin noise flora vertex shader
// Basic shader for rendering textured geometry

cbuffer MatrixBuffer : register(b0)
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

cbuffer GravityBuffer : register(b1)
{
	float gravity;
	float ID;
	float2 padding;
};

struct InputType
{
	float4 position : POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};

struct OutputType
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
	float2 noise = (frac(sin(dot(xy, float2(12.9898, 78.233) * 2.0f)) * 43758.5453));
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

OutputType main(InputType input)
{
	OutputType output;
	float range = 0.5f;
	float noiseValue = 0.0f;
	float4 vPosition = { 0.0f, 0.0f, 0.0f, 1.0f };
	float ID_TREE = 0.0f;
	float ID_BUSH = ID_TREE + 1.0f;
	float ID_GRASS = ID_BUSH + 1.0f;

	// Change the position vector to be 4 units for proper matrix calculations.
	input.position.w = 1.0f;

	// Create a new vertex position based on input position.
	vPosition = mul(vPosition, worldMatrix) + input.position;

	// Calculating the new noise value from gravity.
	noiseValue = (InterpolatedNoise((gravity - range), (gravity + range)) * gravity);

	// If the noise value is greater than 0.
	if (noiseValue > 0.0f)
	{
		//if (ID == ID_TREE)
		//{
		//	// Then divide this new position by a gravity value manipulated by noise.
		//	vPosition.z /= noiseValue;
		//}
		//else if (ID == ID_BUSH)
		//{
			// Then divide this new position by a gravity value manipulated by noise.
			vPosition.y /= noiseValue;
		//}
	}

	// Calculate the position of the vertex against the world, view, and projection matrices.
	output.position = mul(vPosition, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

	// Store the texture coordinates for the pixel shader.
	output.tex = input.tex;

	// Calculate the normal vector against the world matrix only.
	output.normal = mul(input.normal, (float3x3)worldMatrix);

	// Normalize the normal vector.
	output.normal = normalize(output.normal);

	return output;
}