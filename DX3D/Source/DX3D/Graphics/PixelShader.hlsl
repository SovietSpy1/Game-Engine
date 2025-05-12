struct VSOutput
{
    float4 position : SV_Position;
    float4 color : COLOR;
    float2 uvs : TEXCOORD0;
};
float4 main(VSOutput input) : SV_TARGET
{
    float dist = length(input.uvs - float2(0.5, 0.5));
    float glow = exp(-dist * 10); // Controls falloff
    return float4(input.color.rgba);
}