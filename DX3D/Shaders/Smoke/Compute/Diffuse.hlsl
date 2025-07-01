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
StructuredBuffer<float> DataStart : register(t1);

RWStructuredBuffer<float> DataCurrent : register(u0);

[numthreads(10, 10, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
    if (DTid.x > resolution || DTid.y > resolution)
    {
        return;
    }
    float a = diff * resolution * resolution * dt;
    uint x = DTid.x + 1;
    uint y = DTid.y + 1;
    DataCurrent[IX(x, y)] = (DataStart[IX(x, y)] + a * (DataLast[IX(x - 1, y)] + DataLast[IX(x + 1, y)] + DataLast[IX(x, y + 1)] + DataLast[IX(x, y - 1)])) / (4 * a + 1);
}