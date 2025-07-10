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
StructuredBuffer<float> xVel : register(t0);
StructuredBuffer<float> yVel : register(t1);
RWStructuredBuffer<float> Div : register(u0);

[numthreads(10, 10, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
    float h = 1.0f / (float) resolution;
    if (DTid.x >= resolution || DTid.y >= resolution)
    {
        return;
    }
    uint x = DTid.x + 1;
    uint y = DTid.y + 1;
    Div[IX(x,y)] = -0.5f * h * (xVel[IX(x + 1, y)] - xVel[IX(x - 1, y)] + yVel[IX(x, y + 1)] - yVel[IX(x, y - 1)]);
}