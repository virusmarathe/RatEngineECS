struct VS_INPUT
{
	float4 pos : POSITION;
	float2 texcoord : TEXCOORD0;
	float3 normal : NORMAL0;
};

struct VS_OUTPUT
{
	float4 pos : SV_POSITION;
	float2 texcoord : TEXCOORD0;
	float3 normal : TEXCOORD1;
	float3 dirCamera : TEXCOORD2;
};

cbuffer constant : register(b0)
{
	row_major float4x4 m_world;
	row_major float4x4 m_view;
	row_major float4x4 m_projection;
	float4 m_LightDirection;
	float4 m_CameraPosition;
}

VS_OUTPUT vsmain(VS_INPUT input)
{
	VS_OUTPUT output;

	// world space
	output.pos = mul(input.pos, m_world);
	output.dirCamera = normalize(m_CameraPosition.xyz - output.pos);
	// view space
	output.pos = mul(output.pos, m_view);
	// projection space
	output.pos = mul(output.pos, m_projection);

	output.texcoord = input.texcoord;
	output.normal = input.normal;

	return output;
}