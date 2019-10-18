struct VSOUT
{
	float4 pos: SV_POSITION;
	float2 texPos : TEXCOORD;
};

cbuffer VS_INPUT : register(b0)
{
	float2 gresolution;
};
