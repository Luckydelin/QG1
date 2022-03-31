
cbuffer ConstantBuffer : register(b0)//��0���Ĵ���
{
    matrix g_World; // ����
    matrix g_View; // �����
    matrix g_Proj; //ͶӰ
}
struct VertexIn
{
    float3 posL : POSITION;
    float4 color : COLOR;
};

struct VertexOut
{
    float4 posH : SV_POSITION;
    float4 color : COLOR;
};

