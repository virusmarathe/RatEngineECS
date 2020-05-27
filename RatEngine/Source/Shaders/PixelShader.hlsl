Texture2D Texture : register(t0);
sampler TextureSampler: register(s0);

struct PS_INPUT
{
	float4 pos : SV_POSITION;
	float2 texcoord : TEXCOORD0;
	float3 normal : TEXCOORD1;
	float3 dirCamera : TEXCOORD2;
	//float4 color : COLOR;
};

cbuffer constant : register(b0)
{
	row_major float4x4 m_world;
	row_major float4x4 m_view;
	row_major float4x4 m_projection;
	float4 m_LightDirection;
	float4 m_CameraPosition;
}

float4 psmain(PS_INPUT input) : SV_TARGET
{
	float ka = 0.1f;
	float3 ia = float3(1.0f, 1.0f, 1.0f);

	float ambientLight = ka * ia;

	float kd = 0.7f;
	float id = float3(1.0f, 1.0f, 1.0f);
	float ammountDiffuseLight = max(0, dot(-m_LightDirection.xyz, input.normal));

	float diffuseLight = kd * ammountDiffuseLight * id;

	float ks = 1.0f;
	float3 is = float3(1.0f, 1.0f, 1.0f);
	float3 reflectedLight = reflect(m_LightDirection.xyz, input.normal);
	float shininess = 30.0f;
	float ammountSpecularLight = pow(max(0, dot(reflectedLight, input.dirCamera)), shininess);
	float3 specularLight = ks * ammountSpecularLight * is;

	float3 finalLight = ambientLight + diffuseLight + specularLight;

	return Texture.Sample(TextureSampler, input.texcoord * 0.5f) * float4(finalLight, 1.0);
}