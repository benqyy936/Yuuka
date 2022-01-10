struct VS_INPUT
{
	float4 pos:POSITION;
	float4 color:COLOR;
};

struct VS_OUTPUT
{
	float4 pos:SV_POSITION;
	float4 color:COLOR;
};

VS_OUTPUT VS(VS_INPUT vsInput)
{
	VS_OUTPUT vsOut;
	vsOut.pos = vsInput.pos;
	vsOut.color = vsInput.color;
	return vsOut;
}

