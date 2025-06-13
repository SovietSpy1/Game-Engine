struct VSOutput
{
    float4 position : SV_Position;
    float2 texcoord : TEXCOORD0;
};
Texture2D Texture : register(t0);
sampler TextureSampler : register(s0);


float4 main(VSOutput input) : SV_TARGET
{
    return Texture.Sample(TextureSampler, input.texcoord);
}