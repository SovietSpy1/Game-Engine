[numthreads(1, 1, 1)]
void main( uint3 DTid : SV_DispatchThreadID )
{
    /*(sharedDensity[lx][ly] = DensityLast[IX(x, y)];
    if (lx == 1)
    {
        sharedDensity[lx - 1][ly] = DensityLast[IX(x - 1, y)];

        if (ly == 1)
        {
            sharedDensity[lx][ly - 1] = DensityLast[IX(x, y - 1)];
            sharedDensity[lx - 1][ly - 1] = DensityLast[IX(x - 1, y - 1)];
        }
        else if (ly == BLOCK_SIZE)
        {
            sharedDensity[lx][ly + 1] = DensityLast[IX(x, y + 1)];
            sharedDensity[lx - 1][ly + 1] = DensityLast[IX(x - 1, y + 1)];
        }
    }
    else if (lx == BLOCK_SIZE)
    {
        sharedDensity[lx + 1][ly] = DensityLast[IX(x + 1, y)];

        if (ly == 1)
        {
            sharedDensity[lx][ly - 1] = DensityLast[IX(x, y - 1)];
            sharedDensity[lx + 1][ly - 1] = DensityLast[IX(x + 1, y - 1)];
        }
        else if (ly == BLOCK_SIZE)
        {
            sharedDensity[lx][ly + 1] = DensityLast[IX(x, y + 1)];
            sharedDensity[lx + 1][ly + 1] = DensityLast[IX(x + 1, y + 1)];
        }
    }
    else
    {
        if (ly == 1)
        {
            sharedDensity[lx][ly - 1] = DensityLast[IX(x, y - 1)];
        }
        else if (ly == BLOCK_SIZE)
        {
            sharedDensity[lx][ly + 1] = DensityLast[IX(x, y + 1)];
        }
    }
    GroupMemoryBarrierWithGroupSync();*/
}