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
Texture3D<float> Density : register(t0);
RWTexture3D<float4> Colors : register(u0);

[numthreads(BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE)]
void main(uint3 DTid : SV_DispatchThreadID)
{
    if (DTid.x >= resolution || DTid.y >= resolution || DTid.z >= resolution)
    {
        return;
    }
    uint3 coord = uint3(DTid.x, DTid.y, DTid.z);
    Colors[coord] = float4(0.0, 0.0, 0.0, Density[uint3(DTid.x+1, DTid.y+1, DTid.z + 1)]);
}