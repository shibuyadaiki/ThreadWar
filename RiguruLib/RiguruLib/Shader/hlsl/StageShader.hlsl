matrix g_matW;
matrix g_matV;
matrix g_matP;

// テクスチャー
Texture2D g_Tex;

// サンプラーステート
SamplerState  g_Sampler;

float4 g_vecLight;

float4 materialColor;

float4 blendLevel;
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


	float4 pos = In.pos;
	float3 nor = In.normal;

		pos = mul(pos, g_matW);
	Out.pos = mul(pos, g_matV);
	Out.pos = mul(Out.pos, g_matP);



	nor = normalize(nor);
	nor = mul(nor, g_matW);
	Out.normal = mul(nor, g_matV);

	Out.texel = float2(In.texel.x, 1 - In.texel.y);

	return Out;
}

// ピクセルシェーダ
float4 PS_Texture(VS_OUT In) : SV_TARGET
{
	// ランバート拡散照明
	float Color = dot(-normalize(g_vecLight.xyz), normalize(In.normal));
	float4 dif = g_Tex.Sample(g_Sampler, In.texel);
		float3 c = dif.rgb  *max(Color, 0.3f);
		return float4(c, dif.a);
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
		SetPixelShader(CompileShader(ps_5_0, PS_Texture()));
		SetComputeShader(NULL);
	}
}