#define IX(x,y) ((y) * (resolution+2) + x)
cbuffer constant : register(b0)
{
    int resolution;
    float dt;
    float diff;
    float visc;
    uint2 emissionPoint;
    int emissionRadius;
    float emission;
    float max;
    float min;
    int b;
}
StructuredBuffer<float> DataLast : register(t0);

RWStructuredBuffer<float> DataCurrent : register(u0);

[numthreads(10, 10, 1)]
void main( uint3 DTid : SV_DispatchThreadID )
{
    uint x = DTid.x + 1;
    uint y = DTid.y + 1;
    if (DTid.x >= resolution || DTid.y >= resolution)
    {
        return;
    }
    DataCurrent[IX(x, y)] = DataLast[IX(x, y)];
    float dist = distance(uint3(DTid.x, DTid.y, DTid.z), uint3(emissionPoint, 1));
    if ( dist < emissionRadius)
    {
        float dens = (1.0f - dist / (float) emissionRadius);
        uint x = DTid.x + 1;
        uint y = DTid.y + 1;
        DataCurrent[IX(x, y)] = clamp(DataLast[IX(x, y)] + dens * emission * dt, min, max);
    }
}