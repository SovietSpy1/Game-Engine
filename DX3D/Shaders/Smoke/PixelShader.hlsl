struct VSOutput
{
    float4 position : SV_Position;
    float2 texcoord : TEXCOORD0;
};
Texture2D Texture : register(t0);
sampler TextureSampler : register(s0);

cbuffer constant : register(b0)
{
    row_major float4x4 m_world;
    row_major float4x4 m_view;
    row_major float4x4 m_proj;
}
float4 main(VSOutput input) : SV_TARGET
{
    float4 texColor = Texture.Sample(TextureSampler, input.texcoord);
    return texColor;
}