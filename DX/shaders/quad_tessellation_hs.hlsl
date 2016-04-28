// Jason Mottershead, 1300455.

// Quad tessellation hull shader file.
// This will pass along position, texture and normal values for the domain and pixel shader.
// As well as apply tessellation factor to out mesh.

// Tessellation buffer.
cbuffer TessellationBuffer : register(b0)
{
    float tessellationFactor;
    float3 padding;
};

// Hull input.
struct InputType
{
    float4 position : POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};

// Hull constant output.
struct ConstantOutputType
{
    float edges[4] : SV_TessFactor;
    float inside[2] : SV_InsideTessFactor;
};

// Hull output / domain input.
struct OutputType
{
    float4 position : POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};

ConstantOutputType PatchConstantFunction(InputPatch<InputType, 4> inputPatch, uint patchId : SV_PrimitiveID)
{    
    ConstantOutputType output;

    // Set the tessellation factors for the four edges of the quad.
    output.edges[0] = tessellationFactor;
	output.edges[1] = tessellationFactor;
	output.edges[2] = tessellationFactor;
	output.edges[3] = tessellationFactor;

    // Set the tessellation factor for tessallating inside the quad.
    output.inside[0] = tessellationFactor;
	output.inside[1] = tessellationFactor;

    return output;
}

[domain("quad")]
[partitioning("integer")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(4)]
[patchconstantfunc("PatchConstantFunction")]
OutputType main(InputPatch<InputType, 4> patch, uint pointId : SV_OutputControlPointID, uint patchId : SV_PrimitiveID)
{
    OutputType output;
	
    // Set the position for this control point as the output position.
    output.position = patch[pointId].position;

	// Passing the texture coordinates to the domain shader.
	output.tex = patch[pointId].tex;

	// Passing the normal values to the domain shader.
	output.normal = patch[pointId].normal;

    return output;
}