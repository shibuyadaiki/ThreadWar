matrix g_matW;
matrix g_matV;
matrix g_matP;

// �e�N�X�`���[
Texture2D g_Tex;

// �T���v���[�X�e�[�g
SamplerState  g_Sampler;

float4 g_vecLight;

float4 materialColor;

float4x4 boneMatrix[256];
// ���_�V�F�[�_�[�̓��̓p�����[�^
struct VS_IN
{
	float4 pos   : POSITION;   // ���_���W
	float3 normal : NORMAL;      // ���_�J���[
	float2 texel : TEXCOORD;   // �e�N�Z��
	uint4 boneIndex : BONEINDEX;
	uint4 weight : WEIGHT;
};

// ���_�V�F�[�_�[�̏o�̓p�����[�^
struct VS_OUT
{
	float4 pos   : SV_POSITION;
	float3 normal : NORMAL;
	float2 texel : TEXCOORD0;
};

// ���_�V�F�[�_�[
VS_OUT VS_Main(VS_IN In)
{
	VS_OUT Out;
	Out = (VS_OUT)0;

	float4 pos = In.pos;
	float3 nor = In.normal;
	
	pos = In.pos;
	nor = In.normal;

	float4x4 bm0 = boneMatrix[In.boneIndex[0]];
	float4x4 bm1 = boneMatrix[In.boneIndex[1]];
	float4x4 bm2 = boneMatrix[In.boneIndex[2]];
	float4x4 bm3 = boneMatrix[In.boneIndex[3]];

	float3 bpos0 = In.weight[0] * mul(pos, bm0);
	float3 bpos1 = In.weight[1] * mul(pos, bm1);
	float3 bpos2 = In.weight[2] * mul(pos, bm2);
	float3 bpos3 = In.weight[3] * mul(pos, bm3);

	float3 bnor0 = In.weight[0] * mul(nor, bm0);
	float3 bnor1 = In.weight[1] * mul(nor, bm1);
	float3 bnor2 = In.weight[2] * mul(nor, bm2);
	float3 bnor3 = In.weight[3] * mul(nor, bm3);

	pos.xyz = (bpos0.xyz + bpos1.xyz + bpos2.xyz + bpos3.xyz) / 100.0;
	nor.xyz = (bnor0.xyz + bnor1.xyz + bnor2.xyz + bnor3.xyz) / 100.0;
	pos = mul(pos, g_matW);
	Out.pos = mul(pos, g_matV);
	Out.pos = mul(Out.pos, g_matP);

	nor = mul(nor, g_matW);
	nor = mul(nor, g_matV);
	Out.normal = normalize(nor);

	Out.texel = float2(In.texel.x, 1 - In.texel.y);

	return Out;
}

// ���_�V�F�[�_�[
VS_OUT VS_MainNonAni(VS_IN In)
{
	VS_OUT Out;
	Out = (VS_OUT)0;


	float4 pos = In.pos;
	float3 nor = In.normal;

		pos = mul(pos, g_matW);
	Out.pos = mul(pos, g_matV);
	Out.pos = mul(Out.pos, g_matP);

	Out.normal = normalize(nor);
	Out.texel = float2(In.texel.x, 1 - In.texel.y);

	return Out;
}

// �s�N�Z���V�F�[�_
float4 PS_Texture(VS_OUT In) : SV_TARGET
{
	// �����o�[�g�g�U�Ɩ�
	float Color = dot(-normalize(g_vecLight.xyz), normalize(In.normal));
	float4 dif = g_Tex.Sample(g_Sampler, In.texel);
	float3 c = dif.rgb  *max(Color,0.3f);
	return float4(c,dif.a);
}

// �s�N�Z���V�F�[�_
float4 PS_TextureCrystal(VS_OUT In) : SV_TARGET
{
	float4  c = g_Tex.Sample(g_Sampler, In.texel) * materialColor;
		return c;
}

// �G���g���[�|�C���g�ƃo�[�W�������w�肷��
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
	pass P1
	{
		SetVertexShader(CompileShader(vs_5_0, VS_MainNonAni()));
		SetGeometryShader(NULL);
		SetHullShader(NULL);
		SetDomainShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS_Texture()));
		SetComputeShader(NULL);
	}
	pass P2
	{
		SetVertexShader(CompileShader(vs_5_0, VS_MainNonAni()));
		SetGeometryShader(NULL);
		SetHullShader(NULL);
		SetDomainShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS_TextureCrystal()));
		SetComputeShader(NULL);
	}
}