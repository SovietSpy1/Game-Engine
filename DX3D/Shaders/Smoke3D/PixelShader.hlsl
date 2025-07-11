struct VSOutput
{
    float4 position : SV_Position;
    float3 texcoord : TEXCOORD0;
};
Texture2D BackTexture : register(t0);
sampler BackSampler : register(s0);
Texture3D Texture : register(t1);
sampler Texture3DSampler : register(s1);

cbuffer constant : register(b0)
{
    row_major float4x4 m_world;
    row_major float4x4 m_view;
    row_major float4x4 m_proj;
}
float4 main(VSOutput input) : SV_TARGET
{
    float4 texColor = float4(0.0f, 1.0f, 0.0f, 1.0f);
    return texColor;
}