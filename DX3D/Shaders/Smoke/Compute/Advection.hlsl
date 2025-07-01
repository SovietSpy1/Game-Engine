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
StructuredBuffer<float> DataLast : register(t0);
StructuredBuffer<float> VXLast : register(t1);
StructuredBuffer<float> VYLast : register(t2);

RWStructuredBuffer<float> DataCurrent : register(u0);

[numthreads(BLOCK_SIZE, BLOCK_SIZE, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
    uint x = DTid.x + 1;
    uint y = DTid.y + 1;
    if (x > resolution || y > resolution)
    {
        return;
    }
    float xPos = x - VXLast[IX(x, y)] * dt * resolution;
    xPos = clamp(xPos, 0.0f, (float) resolution);
    float yPos = y - VYLast[IX(x, y)] * dt * resolution;
    yPos = clamp(yPos, 0.0f, (float) resolution);
    int i = (int) xPos;
    int j = (int) yPos;
    float iProp = xPos - i;
    float jProp = yPos - j;
    float bL = (1 - iProp) * (1 - jProp);
    float bR = (iProp) * (1 - jProp);
    float tL = (1 - iProp) * (jProp);
    float tR = (iProp) * (jProp);
    
    DataCurrent[IX(x, y)] = bL * DataLast[IX(i, j)] + bR * DataLast[IX(i+1, j)] + tL * DataLast[IX(i, j+1)] + tR * DataLast[IX(i+1, j+1)];
}