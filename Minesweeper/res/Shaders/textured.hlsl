struct vs_in {
    float3 position_local : Position;
    float2 texCoord : TexCoord;
};

struct vs_out {
    float4 position_clip : SV_POSITION;
    float3 pos : Position;
    float2 texCoord : TexCoord;
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
    output.texCoord = input.texCoord;
    return output;
}

Texture2D tex : register(t0);
SamplerState splr : register(s0);

float4 ps_main(vs_out input) : SV_TARGET
{
    float4 texColor = tex.Sample(splr, input.texCoord);
    //if (texColor.a < 0.1)
    //    discard;
    return texColor;
}