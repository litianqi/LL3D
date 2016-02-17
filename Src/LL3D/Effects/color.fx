
cbuffer PerObject
{
	float4x4 g_matrix_wvp;
};

struct VertexIn
{
	float3 pos  : POSITION;
  float4 color : COLOR;
};

struct VertexOut
{
	float4 pos  : SV_POSITION;
  float4 color : COLOR;
};

VertexOut VS(VertexIn vin)
{
	VertexOut vout;
	
	// Transform to homogeneous clip space.
	vout.pos = mul(float4(vin.pos, 1.0f), g_matrix_wvp);
	
	// Just pass vertex color into the pixel shader.
    vout.color = vin.color;
    
    return vout;
}

float4 PS(VertexOut pin) : SV_Target
{
    return pin.color;
}

technique11 ColorTech
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_5_0, VS() ) );
        SetGeometryShader(NULL);
        SetPixelShader( CompileShader( ps_5_0, PS() ) );
    }
}
