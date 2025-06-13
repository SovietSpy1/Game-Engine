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
    float3 normal : NORMAL0;
    float3 direction_to_camera : TEXCOORD1;
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
    output.direction_to_camera = camera_position.xyz - output.position.xyz;
    //VIEW POSITION
    output.position = mul(output.position, m_view);
    //SCREEN SPACE POSITION
    output.position = mul(output.position, m_proj);
    output.texcoord = input.texcoord;
    output.normal = mul(float4(input.normal, 1), m_rotation);
    return output;
}