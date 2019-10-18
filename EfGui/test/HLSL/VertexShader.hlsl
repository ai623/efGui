#include "type.hlsli"

void main(
	float2 pos : POSITION,
	float2 texCord : TEXCOORD, 
	out VSOUT vOut) 
{
	vOut.pos = float4(pos/ gresolution, 0, 1);
	vOut.texPos = texCord;
}