struct VSOutput
{
    float4 position : SV_Position;
    float4 color : COLOR;
};
cbuffer constant : register(b0)
{
    row_major float4x4 m_world;
    row_major float4x4 m_view;
    row_major float4x4 m_proj;
    float elapsedTime;
}

float4 main(VSOutput input) : SV_TARGET
{
    return input.color;
}