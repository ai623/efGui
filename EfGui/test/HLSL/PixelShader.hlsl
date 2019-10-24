#include "type.hlsli"
Texture2D gtex : register(t0);
SamplerState gsampler :register(s0);

float4 main(VSOUT pIn) : SV_TARGET
{
	float4 color = gtex.Sample(gsampler,pIn.texPos);
	color.xyz = 0;
	return color;
}