struct VSInput
{
float3 position : POSITION;
float2 texcoord : TEXCOORD0;
float3 normal : NORMAL0;
float4 color : COLOR0;
};
struct VSOutput
{
    float4 position : SV_Position;
};
VSOutput main(VSInput input)
{
    VSOutput output;
    output.position = float4(input.position, 1);
    return output;
}