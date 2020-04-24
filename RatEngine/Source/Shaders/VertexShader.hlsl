struct VS_INPUT
{
	float4 pos : POSITION;
	float4 color : COLOR;
	float4 color1 : COLOR1;
};

struct VS_OUTPUT
{
	float4 pos : SV_POSITION;
	float4 color : COLOR;
	float4 color1 : COLOR1;
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

	output.color = input.color;
	output.color1 = input.color1;

	return output;
}