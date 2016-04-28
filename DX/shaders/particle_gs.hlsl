// Jason Mottershead, 1300455.

// Particle geometry shader file.
// This will output a shape based on a point.

// Matrix buffer.
cbuffer MatrixBuffer : register(b0)
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};

// Position buffer.
cbuffer PositionBuffer : register(b1)
{
	static float3 gPositions[4] =		
	{
		float3(0.0f, 0.0f, 0.0f),		// Bottom left.
		float3(1.0f, 0.0f, 0.0f),		// Bottom right.
		float3( 0.0f, 1.0f, 0.0f ),		// Top left.
		float3( 1.0f, 1.0f, 0.0f)		// Top right.
	};
	float3 translation;
	float padding;
	float4 positionPadding;		
};

// Geometry input.
struct InputType
{
	float4 position : POSITION;
	float4 colour : COLOR;
};

// Geometry output / pixel input.
struct OutputType
{
	float4 position : SV_POSITION;
	float4 colour : COLOR;
};

[maxvertexcount(4)]
void main(point InputType input[1], inout TriangleStream<OutputType> triStream)
{

	OutputType output;
	int numberOfVertices = 4;
	float3 vPosition = { 0.0f, 0.0f, 0.0f };

	// Change the position vector to be 4 units for proper matrix calculations.
	input[0].position.w = 1.0f;

	// Used for creating the positions for the points.
	for (int i = 0; i < numberOfVertices; i++)
	{
		// Place each vertex into vPosition.
		vPosition = gPositions[i];

		// Multiply the result by the world matrix, and add on vertex input.
		vPosition = mul(vPosition, (float3x3) worldMatrix) + input[0].position;

		// Increase the size of the shape by 2.
		vPosition.x *= 2;
		vPosition.y *= 2;
		vPosition.z *= 2;

		// Move the particles where we want them to be.
		vPosition.x += translation.x;
		vPosition.y += translation.y;
		vPosition.z += translation.z;

		// Calculate the output position based on the vertex.
		output.position = mul(float4(vPosition, 1.0f), worldMatrix);
		output.position = mul(output.position, viewMatrix);
		output.position = mul(output.position, projectionMatrix);

		// Calculate the output colour of the particle.
		output.colour = input[0].colour;

		triStream.Append(output);
	}

	triStream.RestartStrip();

}