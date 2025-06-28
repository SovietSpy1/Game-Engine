cbuffer constant : register(b0)
{
    int resolution;
    float dt;
    float diff;
    float visc;
    uint2 emissionPoint;
    float2 emissionVelocity;
    int emissionRadius;
    float emissionDensity;
}
[numthreads(10, 10, 1)]
void main( uint3 DTid : SV_DispatchThreadID )
{
    if (distance(uint3(DTid.x + 1, DTid.y + 1, DTid.z), uint3(emissionPoint, 1)) < emissionRadius)
    {
        
    }

}