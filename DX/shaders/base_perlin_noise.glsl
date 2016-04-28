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

float CreatePerlinNoise(float x, float y)
{
	float result = 0.0f, amplitude = 0.0f, frequency = 0.0f, persistance = 0.1f;

	for (int i = 1; i <= 4; i++)
	{
		frequency += 2;
		amplitude += persistance;
		result += InterpolatedNoise(x * frequency, y * frequency) * amplitude;
	}

	return result;
}
