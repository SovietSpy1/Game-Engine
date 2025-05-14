struct VSInput
{
    float3 position: POSITION;
    float4 color : COLOR;
    float2 uvs : TEXCOORD0;
};
struct VSOutput
{
    float4 position : SV_Position;
    float4 color : COLOR;
    float2 uvs : TexCOORD0;
    float4 screenPos : TEXCOORD1;
};
cbuffer constant : register(b0)
{
    row_major float4x4 m_world;
    row_major float4x4 m_view;
    row_major float4x4 m_proj;
    float elapsedTime;
}

VSOutput main( VSInput input )
{
    VSOutput output;
    //WORLD POSITION
    output.position = mul(float4(input.position, 1.0f), m_world);
    //VIEW POSITION
    output.position = mul(output.position, m_view);
    //SCREEN SPACE POSITION
    output.position = mul(output.position, m_proj);
    output.color = input.color;
    output.uvs = input.uvs;
    output.screenPos = float4(input.position, 1.0);
    return output;
}