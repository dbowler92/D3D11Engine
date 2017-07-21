//Blits an SRV in to an RTV
//

//The texture to copy in to the RTV
Texture2D textureToBlit : register(t0);

//Output from the VS to the PS
struct VSOutput
{
    float4 V_Position : SV_Position;
};

float4 main(VSOutput input) : SV_Target
{
    float4 sample = textureToBlit.Load(int3(input.V_Position.xy, 0));
    return sample;
}