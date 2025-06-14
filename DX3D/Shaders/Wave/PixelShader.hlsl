struct VSOutput
{
    float4 position : SV_Position;
    float2 texcoord : TEXCOORD0;
    float3 normal : NORMAL0; // Normal vector for lighting calculations
    float3 direction_to_camera : TEXCOORD1; // Direction to the camera for specular calculations
};
Texture2D Texture : register(t0);
sampler TextureSampler : register(s0);

cbuffer constant : register(b0)
{
    row_major float4x4 m_world;
    row_major float4x4 m_view;
    row_major float4x4 m_proj;
    float4 light_direction;
}

float4 main(VSOutput input) : SV_TARGET
{
    float4 texColor = Texture.Sample(TextureSampler, input.texcoord);
    float ka = 0.1; // Ambient light coefficient
    float3 ia = texColor;
    float3 ambient_light = ka * ia;
    
    float kd = 0.9; // Diffuse light coefficient
    float3 id = texColor;
    float amount_diffuse_light = max(0.0, dot(light_direction.xyz, -input.normal));
    float3 diffuse_light = kd * id * amount_diffuse_light;
    //specular
    float ks = 1.0;
    float3 is = float3(1.0, 1.0, 1.0);
    float3 reflected_light = normalize(reflect(light_direction.xyz, input.normal));
    float shininess = 32;
    float specular_mag = pow(max(0.0, dot(reflected_light, normalize(input.direction_to_camera))), shininess);
    float3 specular_light = ks * specular_mag * is;
    float3 final_light = ambient_light + diffuse_light + specular_light;
    return float4(texColor.xyz, 0.5f);
}