#define BLOCK_SIZE 10
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
StructuredBuffer<float> Density : register(t0);
RWStructuredBuffer<float4> Colors : register(u0);

[numthreads(BLOCK_SIZE, BLOCK_SIZE, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
    if (DTid.x >= resolution || DTid.y >= resolution)
    {
        return;
    }
    uint2 coord = uint2(DTid.x, DTid.y);
    Colors[0] = float4(0.0, 0.0, 0.0, Density[IX(DTid.x+1, DTid.y+1)]);
}