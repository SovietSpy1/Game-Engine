#define IX(x,y) ((y) * (resolution+2) + x)
cbuffer constant : register(b0)
{
    int resolution;
    float dt;
    float diff;
    float visc;
    uint2 emissionPoint;
    float emissionRadius;
    float emission;
    float max;
    float min;
    int b;
}
StructuredBuffer<float> DataLast : register(t0);
StructuredBuffer<float> Pressure : register(t1);
RWStructuredBuffer<float> DataCurrent : register(u0);

[numthreads(10, 10, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
    if (DTid.x >= resolution || DTid.y >= resolution)
    {
        return;
    }
    float h = 1.0f / (float) resolution;
    uint x = DTid.x + 1;
    uint y = DTid.y + 1;
    if (b == 1)
    {
        DataCurrent[IX(x, y)] = DataLast[IX(x, y)] - ((Pressure[IX(x + 1, y)] - Pressure[IX(x - 1, y)]) * 0.5f / h);
    }
    else if (b == 2)
    {
        DataCurrent[IX(x, y)] = DataLast[IX(x, y)] - ((Pressure[IX(x, y+1)] - Pressure[IX(x, y-1)]) * 0.5f / h);
    }
        
}