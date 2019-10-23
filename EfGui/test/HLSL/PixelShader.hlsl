#include "type.hlsli"
Texture2D gtex : register(t0);
SamplerState gsampler :register(s0);

float4 main(VSOUT pIn) : SV_TARGET
{
	float4 color = gtex.Sample(gsampler,pIn.texPos);
	
	return color;
	/*float2 pos = pIn.pos.xy / gresolution;

	uint texWidth,texHeight,texNumLevels;
	uint4 color;
	uint2 dimentions;
	gtex.GetDimensions(dimentions.x, dimentions.y);
	color = gtex[pos * dimentions];
	return color;*/
	//return float4(1.0f, 1.0f, 1.0f, 1.0f);
}