struct VSOutput
{
    float4 position : SV_Position;
    float4 color : COLOR0;
};
float4 main(VSOutput input) : SV_TARGET
{
    return input.color;
}