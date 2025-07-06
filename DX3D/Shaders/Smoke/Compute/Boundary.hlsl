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
    uint i = 0;
    if (DTid.x > 4 * resolution + 3)
    {
        return;
    }
    float data = DataLast[DTid.x];
    if (DTid.x < resolution)
    {
        i = DTid.x + 1;
        DataCurrent[IX(0, i)] = b == 1 ? -1 * data : data;
    }
    else if (DTid.x < 2 * resolution) // top wall
    {
        i = DTid.x - resolution + 1;
        DataCurrent[IX(i, resolution + 1)] = b == 2 ? -1 * data : data;
    }
    else if (DTid.x < 3 * resolution) // right wall
    {
        i = DTid.x - 2 * resolution + 1;
        DataCurrent[IX(resolution+ 1, i)] = b == 1 ? -1 * data : data;
    }
    else if (DTid.x < 4 * resolution) // bottom wall
    {
        i = DTid.x - 3 * resolution + 1;
        DataCurrent[IX(i, 0)] = b == 2 ? -1 * data : data;
    }
    else if (DTid.x == 4 * resolution)
    {
        DataCurrent[IX(0, 0)] = b != 0 ? -data : data;
    }
    else if (DTid.x == 4 * resolution + 1)
    {
        DataCurrent[IX(0, resolution + 1)] = b != 0 ? -data : data;
    }
    else if (DTid.x == 4 * resolution + 2)
    {
        DataCurrent[IX(resolution + 1, resolution + 1)] = b != 0 ? -data : data;
    }
    else if (DTid.x == 4 * resolution + 3)
    {
        DataCurrent[IX(resolution + 1, 0)] = b != 0 ? -data : data;
    }
}