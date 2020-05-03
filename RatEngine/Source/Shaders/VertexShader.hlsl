struct VS_INPUT
{
	float4 pos : POSITION;
	float2 texcoord : TEXCOORD0;
	float4 color : COLOR;
};

struct VS_OUTPUT
{
	float4 pos : SV_POSITION;
	float2 texcoord : TEXCOORD0;
	float4 color : COLOR;
};

cbuffer constant : register(b0)
{
	row_major float4x4 m_world;
	row_major float4x4 m_view;
	row_major float4x4 m_projection;
	unsigned int m_time;
}

VS_OUTPUT vsmain(VS_INPUT input)
{
	VS_OUTPUT output;

	// world space
	output.pos = mul(input.pos, m_world);
	// view space
	output.pos = mul(output.pos, m_view);
	// projection space
	output.pos = mul(output.pos, m_projection);

	output.texcoord = input.texcoord;
	output.color = input.color;

	return output;
}