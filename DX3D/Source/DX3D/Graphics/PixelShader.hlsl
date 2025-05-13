struct VSOutput
{
    float4 position : SV_Position;
    float4 color : COLOR;
    float2 uvs : TEXCOORD0;
    float4 screenPos : TEXCOORD1;
};
cbuffer ConstantBuffer : register(b0)
{
    float elapsedTime;
}

float4 main(VSOutput input) : SV_TARGET
{
    float2 ndc = input.screenPos.xy / input.screenPos.w;
    float dist = distance(ndc, (0, 0));
    float glow = exp(-dist * (3 - sin(3.14159 * elapsedTime)));
    return float4(input.color.r + 0.3 * sin(3.14159 * elapsedTime), input.color.g, input.color.b, input.color.a * glow);
}