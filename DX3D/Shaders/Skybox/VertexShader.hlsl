struct VSInput
{
    float3 position : POSITION;
    float2 texcoord : TEXCOORD0;
    float3 normal : NORMAL0;
};
struct VSOutput
{
    float4 position : SV_Position;
    float2 texcoord : TEXCOORD0;
};
cbuffer constant : register(b0)
{
    row_major float4x4 m_world;
    row_major float4x4 m_view;
    row_major float4x4 m_proj;
    float4 light_direction; // Direction of the light source
    float4 camera_position; // Position of the camera
    row_major float4x4 m_rotation;
}

VSOutput main(VSInput input)
{
    VSOutput output;
    //WORLD POSITION
    output.position = mul(float4(input.position, 1.0f), m_world);
    //VIEW POSITION
    output.position = mul(output.position, m_view);
    //SCREEN SPACE POSITION
    output.position = mul(output.position, m_proj);
    output.texcoord = float2(input.texcoord.x, (1 - input.texcoord.y));
    return output;
}