struct VSInput
{
    float3 position : POSITION;
    float3 texcoord : TEXCOORD0;
    float3 normal : NORMAL0;
    float4 color : COLOR0;
};
struct VSOutput
{
    float4 position : SV_Position;
    float3 texcoord : TEXCOORD0;
};
cbuffer constant : register(b0)
{
    row_major float4x4 m_world;
    row_major float4x4 m_view;
    row_major float4x4 m_proj;
}

VSOutput main(VSInput input)
{
    VSOutput output;
    //WORLD POSITION
    output.position = mul(float4(input.position, 1.0f), m_world);
    //VIEW POSITION
    output.position = mul(output.position, m_view);
    //SCREEN SPACE POSITION
    output.position = mul(output.position, m_proj);
    output.texcoord = input.texcoord;
    return output;
}