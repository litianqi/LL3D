SamplerState kSamLinear
{
  Filter = MIN_MAG_MIP_LINEAR;
  AddressU = Wrap;
  AddressV = Wrap;
};

DepthStencilState kDisableDepth
{
  DepthEnable = FALSE;
  DepthWriteMask = ZERO;
};

DepthStencilState kNoDepthWrites
{
  DepthEnable = TRUE;
  DepthWriteMask = ZERO;
};

BlendState kAdditiveBlending
{
  AlphaToCoverageEnable = FALSE;
  BlendEnable[0] = TRUE;
  SrcBlend = SRC_ALPHA;
  DestBlend = ONE;
  BlendOp = ADD;
  SrcBlendAlpha = ZERO;
  DestBlendAlpha = ZERO;
  BlendOpAlpha = ADD;
  RenderTargetWriteMask[0] = 0x0F;
};