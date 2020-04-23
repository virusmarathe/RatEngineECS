struct VS_INPUT
{
	float4 pos : POSITION;
	float4 pos1 : POSITION1;
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
	unsigned int m_time;
}

VS_OUTPUT vsmain(VS_INPUT input)
{
	VS_OUTPUT output;
	output.pos = lerp(input.pos, input.pos1, (sin(m_time/1000.0f) + 1) / 2.0f);
	output.color = input.color;
	output.color1 = input.color1;

	return output;
}