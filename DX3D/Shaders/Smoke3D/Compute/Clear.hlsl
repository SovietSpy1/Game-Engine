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
RWStructuredBuffer<float> Data : register(u0);

[numthreads(10, 10, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
    if (DTid.x >= resolution || DTid.y >= resolution)
    {
        return;
    }
    uint x = DTid.x + 1;
    uint y = DTid.y + 1;
    Data[IX(x, y)] = 0.0;
}