struct VS_OUTPUT
{
	float4 pos:SV_POSITION;
	float4 color:COLOR;
};

float4 PS(VS_OUTPUT psInput) : SV_TARGET
{
	return psInput.color;
}