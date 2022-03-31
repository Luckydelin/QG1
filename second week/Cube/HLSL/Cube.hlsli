
cbuffer ConstantBuffer : register(b0)//第0个寄存器
{
    matrix g_World; // 世界
    matrix g_View; // 摄像机
    matrix g_Proj; //投影
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

