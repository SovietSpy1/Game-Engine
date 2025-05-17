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
    float glow = exp(-1 - sin(2 * 3.14159 * elapsedTime));
    return float4(input.color.r + 0.1 * sin(2 * 3.14159 * elapsedTime), input.color.g, input.color.b, input.color.a);
}