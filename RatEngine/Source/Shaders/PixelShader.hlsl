struct PS_INPUT
{
	float4 pos : SV_POSITION;
	float4 color : COLOR;
};

float4 psmain(PS_INPUT input) : SV_TARGET
{
	input.color.r = 1;
	return input.color;
}