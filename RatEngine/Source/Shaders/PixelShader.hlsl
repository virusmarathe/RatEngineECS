struct PS_INPUT
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

float4 psmain(PS_INPUT input) : SV_TARGET
{
	return lerp(input.color, input.color1, (sin(m_time/1000.0f) + 1) / 2.0f);
}