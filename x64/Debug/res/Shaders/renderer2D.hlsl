struct vs_in {
    float3 position_local : Position;
    float4 color : Color;
};

struct vs_out {
    float4 position_clip : SV_POSITION;
    float3 pos : Position;
    float4 color : Color;
};

cbuffer Cbuff : register(b1)
{
    matrix transform;
};

vs_out vs_main(vs_in input)
{
    vs_out output = (vs_out)0;
    output.position_clip = mul(float4(input.position_local, 1.0), transform);
    output.pos = input.position_local;
    output.color = input.color;
    return output;
}

float4 ps_main(vs_out input) : SV_TARGET
{
    return float4(input.color);
}