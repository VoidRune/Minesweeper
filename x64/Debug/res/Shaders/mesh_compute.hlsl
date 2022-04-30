struct Data
{
    float4 meshVertex;
    float m;
};

cbuffer Cbuff : register(b7)
{
    float2 tileOffset;
    float2 win;
    float2 tile;
    float2 padding;
};

StructuredBuffer<int>   inputArray  : register(t0);
RWStructuredBuffer<int> outputArray : register(u0);

[numthreads(1024, 1, 1)]
void cs_main(uint3 id : SV_DispatchThreadID)
{
    int index = id.x + 64 * id.y;
    outputArray[index] = 2 * inputArray[index];


    //outputArray[index    ].meshVertex = float4((-tileOffset.x + (float)id.x) * tile.x / win.x, (-tileOffset.y + (float)id.y)* tile.x / win.y, 5, t / tile.y);
    //outputArray[index    ].m = 0;
    //outputArray[index + 1].meshVertex = float4((-tileOffset.x + (float)id.x) * tile.x / win.x, (-tileOffset.y + (float)id.y)* tile.x / win.y + tile.x / win.y, 5, t / tile.y);
    //outputArray[index + 1].m = 1;
    //outputArray[index + 2].meshVertex = float4((-tileOffset.x + (float)id.x) * tile.x / win.x + tile.x / win.x, (-tileOffset.y + (float)id.y)* tile.x / win.y + tile.x / win.y, 5, (t + 1) / tile.y);
    //outputArray[index + 2].m = 1;
    //outputArray[index + 3].meshVertex = float4((-tileOffset.x + (float)id.x) * tile.x / win.x + tile.x / win.x, (-tileOffset.y + (float)id.y) * tile.x / win.y, 5, (t + 1) / tile.y);
    //outputArray[index + 3].m = 0;

}