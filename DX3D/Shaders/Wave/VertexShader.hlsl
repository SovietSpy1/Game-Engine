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
    float elapsedTime;
}

VSOutput main(VSInput input)
{
    VSOutput output;
    //WORLD POSITION
    float ChangeInY = sin(3.14 * (input.position.z + elapsedTime) / 4.0f) / 10;
    float3 modifiedPosition = input.position + float3(0, ChangeInY, 0);
    output.position = mul(float4(modifiedPosition, 1.0f), m_world);
    output.direction_to_camera = camera_position.xyz - output.position.xyz;
    //VIEW POSITION
    output.position = mul(output.position, m_view);
    //SCREEN SPACE POSITION
    output.position = mul(output.position, m_proj);
    float2 texcoord = float2(input.texcoord.x, input.texcoord.y * m_world[2][2]);
    output.texcoord = texcoord;
    output.normal = mul(float4(input.normal, 1), m_rotation);
    return output;
}