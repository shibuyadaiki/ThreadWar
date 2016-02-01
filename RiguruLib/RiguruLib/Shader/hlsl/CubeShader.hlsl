matrix g_matW;
matrix g_matV;
matrix g_matP;

float4 materialColor;
float4 startPos, endPos;
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

// ���_�V�F�[�_�[
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

// �s�N�Z���V�F�[�_
float4 PS_Line(VS_OUT In) : SV_TARGET
{
	return materialColor;
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