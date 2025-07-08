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

[numthreads(10, 1, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
    if (DTid.x > 4 * resolution + 3)
    {
        return;
    }
    int i = DTid.x % resolution + 1;
    if (DTid.x < resolution)
    {
        DataCurrent[DTid.x] = DataLast[IX(1, i)];
    }
    else if (DTid.x < 2 * resolution)
    {
        DataCurrent[DTid.x] = DataLast[IX(i, resolution)];
    }
    else if (DTid.x < 3 * resolution)
    {
        DataCurrent[DTid.x] = DataLast[IX(resolution, i)];
    }
    else if (DTid.x < 4 * resolution)
    {
        DataCurrent[DTid.x] = DataLast[IX(i, 1)];
    }
    else if (DTid.x == resolution * 4)
    {
        DataCurrent[DTid.x] = DataLast[IX(1, 1)];
    }
    else if (DTid.x == resolution * 4 + 1)
    {
        DataCurrent[DTid.x] = DataLast[IX(1, resolution)];
    }
    else if (DTid.x == resolution * 4 + 2)
    {
        DataCurrent[DTid.x] = DataLast[IX(resolution, resolution)];
    }
    else if (DTid.x == resolution * 4 + 3)
    {
        DataCurrent[DTid.x] = DataLast[IX(resolution, 1)];
    }
}