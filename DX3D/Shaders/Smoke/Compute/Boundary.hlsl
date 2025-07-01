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
    uint i = DTid.x % resolution + 1;
    if (DTid.x < resolution)
    {
        DataCurrent[IX(0, i)] = b == 1 ? -1 * DataLast[IX(1, i)] : DataLast[IX(1, i)];
    }
    else if (DTid.x < 2 * resolution)
    {
        DataCurrent[IX(resolution + 1, i)] = b == 1 ? -1 * DataLast[IX(resolution, i)] : DataLast[IX(resolution, i)];
    }
    else if (DTid.x < 3 * resolution)
    {
        DataCurrent[IX(i, 0)] = b == 2 ? -1 * DataLast[IX(i, 1)] : DataLast[IX(i, 1)];
    }
    else if (DTid.x < 4 * resolution)
    {
        DataCurrent[IX(i, resolution + 1)] = b == 2 ? -1 * DataLast[IX(i, resolution)] : DataLast[IX(i, resolution)];
    }
    AllMemoryBarrierWithGroupSync();
    
    if (DTid.x == resolution * 4)
    {
        DataCurrent[IX(0, 0)] = 0.5 * (DataCurrent[IX(1, 0)] + DataCurrent[IX(0, 1)]);
    }
    else if (DTid.x == resolution * 4 + 1)
    {
        DataCurrent[IX(0, resolution + 1)] = 0.5 * (DataCurrent[IX(1, resolution + 1)] + DataCurrent[IX(0, resolution)]);
    }
    else if (DTid.x == resolution * 4 + 2)
    {
        DataCurrent[IX(resolution + 1,0)] = 0.5 * (DataCurrent[IX(resolution, 0)] + DataCurrent[IX(resolution + 1,1)]);
    }
    else if (DTid.x == resolution * 4 + 3)
    {
        DataCurrent[IX(resolution + 1, resolution + 1)] = 0.5 * (DataCurrent[IX(resolution, resolution + 1)] + DataCurrent[IX(resolution + 1, resolution)]);
    }
}