matrix g_matW;
matrix g_matV;
matrix g_matP;

// �e�N�X�`���[
Texture2D g_Tex;

// �T���v���[�X�e�[�g
SamplerState  g_Sampler;

float4 g_vecLight;

float4 materialColor;

float4 blendLevel;
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

		pos = mul(pos, g_matW);
	Out.pos = mul(pos, g_matV);
	Out.pos = mul(Out.pos, g_matP);



	nor = normalize(nor);
	nor = mul(nor, g_matW);
	Out.normal = mul(nor, g_matV);

	Out.texel = float2(In.texel.x, 1 - In.texel.y);

	return Out;
}

// �s�N�Z���V�F�[�_
float4 PS_Texture(VS_OUT In) : SV_TARGET
{
	// �����o�[�g�g�U�Ɩ�
	float Color = dot(-normalize(g_vecLight.xyz), normalize(In.normal));
	float4 dif = g_Tex.Sample(g_Sampler, In.texel);
		float3 c = dif.rgb  *max(Color, 0.3f);
		return float4(materialColor.xyz, dif.a);
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
}