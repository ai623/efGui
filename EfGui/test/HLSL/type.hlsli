struct VSOUT
{
	float4 pos: SV_POSITION;
	float2 texPos : TEXCOORD;
};

cbuffer Camera2DInfo : register(b0)
{
	float2 gratio;
	float2 gbias;
};
