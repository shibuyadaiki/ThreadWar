matrix g_matW;
matrix g_matV;
matrix g_matP;

float4 materialColor;
float4 startPos, endPos;
// 頂点シェーダーの入力パラメータ
struct VS_IN
{
	float4 pos   : POSITION;   // 頂点座標
	float3 normal : NORMAL;      // 頂点カラー
	float2 texel : TEXCOORD;   // テクセル
	uint4 boneIndex : BONEINDEX;
	uint4 weight : WEIGHT;
};

// 頂点シェーダーの出力パラメータ
struct VS_OUT
{
	float4 pos   : SV_POSITION;
	float3 normal : NORMAL;
	float2 texel : TEXCOORD0;
};

// 頂点シェーダー
VS_OUT VS_Main(VS_IN In)
{
	VS_OUT Out;
	Out = (VS_OUT)0;
	float4 pos;

	pos.x = lerp(startPos.x, endPos.x, In.pos.x);
	pos.y = lerp(startPos.y, endPos.y, In.pos.y);
	pos.z = lerp(startPos.z, endPos.z, In.pos.z);
	pos.w = 1;
	pos = mul(pos, g_matW);
	Out.pos = mul(pos, g_matV);
	Out.pos = mul(Out.pos, g_matP);
	return Out;
}

// 頂点シェーダー
VS_OUT VS_MainScale(VS_IN In)
{
	VS_OUT Out;
	Out = (VS_OUT)0;
	float4 pos;

	pos = In.pos;
	pos = mul(pos, g_matW);
	Out.pos = mul(pos, g_matV);
	Out.pos = mul(Out.pos, g_matP);
	return Out;
}

// ピクセルシェーダ
float4 PS_Line(VS_OUT In) : SV_TARGET
{
	return materialColor;
}

// エントリーポイントとバージョンを指定する
technique11 TShader
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS_Main()));
		SetGeometryShader(NULL);
		SetHullShader(NULL);
		SetDomainShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS_Line()));
		SetComputeShader(NULL);
	}
	pass P1
	{
		SetVertexShader(CompileShader(vs_5_0, VS_MainScale()));
		SetGeometryShader(NULL);
		SetHullShader(NULL);
		SetDomainShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS_Line()));
		SetComputeShader(NULL);
	}
}