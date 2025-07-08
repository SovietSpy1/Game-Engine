#define norm(coord) (float3(coord) / (float)(resolution + 2))
cbuffer constant : register(b0)
{
    int resolution;
    float dt;
    float diff;
    float visc;
    uint3 emissionPoint;
    int emissionRadius;
    float emission;
    float max;
    float min;
    int b;
}
Texture3D<float> DataLast : register(t0);
Texture3D<float> VXLast : register(t1);
Texture3D<float> VYLast : register(t2);
Texture3D<float> VZLast : register(t3);
RWTexture3D<float> DataCurrent : register(u0);

SamplerState TriSampler : register(s0);

[numthreads(10, 10, 10)]
void main(uint3 DTid : SV_DispatchThreadID)
{
    if (DTid.x >= resolution || DTid.y >= resolution || DTid.z >= resolution)
    {
        return;
    }
    uint x = DTid.x + 1;
    uint y = DTid.y + 1;
    uint z = DTid.z + 1;
    float3 p = norm(float3(x, y, z));
    float xPos = x - VXLast.SampleLevel(TriSampler, p, 0.0) * dt * resolution;
    xPos = clamp(xPos, 0.0f, (float) resolution);
    float yPos = y - VYLast.SampleLevel(TriSampler, p, 0.0) * dt * resolution;
    yPos = clamp(yPos, 0.0f, (float) resolution);
    float zPos = z - VZLast.SampleLevel(TriSampler, p, 0.0) * dt * resolution;
    zPos = clamp(zPos, 0.0f, (float) resolution);
    
    
    DataCurrent[uint3(x,y,z)] = DataLast.Sample(TriSampler, norm(float3(xPos, yPos, zPos)));

}