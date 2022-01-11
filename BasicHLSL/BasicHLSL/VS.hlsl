#include "Type.hlsli"

VS_OUTPUT VS(VS_INPUT vsInput)
{
	VS_OUTPUT vsOut;
	vsOut.pos = vsInput.pos;
	vsOut.texcoord = vsInput.texcoord;
	return vsOut;
}

