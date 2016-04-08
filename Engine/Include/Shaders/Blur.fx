#define GROUP_THREAD_NUM 256
#define RADIUS 5
#define CACHE_SIZE (GROUP_THREAD_NUM + 2 * RADIUS)

cbuffer Settings
{
  float g_weights[2 * RADIUS + 1] = {
  0.05f, 0.05f, 0.1f, 0.1f, 0.1f, 0.2f, 0.1f, 0.1f, 0.1f, 0.05f, 0.05f
  };
};

Texture2D g_input;
RWTexture2D<float4> g_output;
groupshared float4 g_cache[CACHE_SIZE];

[numthreads(GROUP_THREAD_NUM, 1, 1)]
void CSHorizonalBlur(
  int3 group_thread_id : SV_GroupThreadID, 
  int3 dispatch_id : SV_DispatchThreadID) 
{
  // First, fill g_cache
  
  // Let begin and end RADIUS threads do some extra work so that begin and end  
  // RADIUS g_cache will be initialized.
  if (group_thread_id.x < RADIUS) {
    int x = max(dispatch_id.x - RADIUS, 0);
    g_cache[group_thread_id.x] = g_input[int2(x, dispatch_id.y)];
  }
    
  if (group_thread_id.x >= GROUP_THREAD_NUM - RADIUS) {
    int x = min(dispatch_id.x + RADIUS, g_input.Length.x - 1);
    g_cache[group_thread_id.x + 2 * RADIUS] = g_input[int2(x, dispatch_id.y)];
  }

  g_cache[group_thread_id.x + RADIUS] = 
    g_input[min(dispatch_id.xy, g_input.Length.xy - int2(1, 0))];

  // Wait for all threads to finish.
  GroupMemoryBarrierWithGroupSync();
  
  // Second, do blur
  
  float4 result;
  for (int i = -RADIUS; i <= RADIUS; i++) {
    result += g_weights[i + RADIUS] * g_cache[group_thread_id.x + RADIUS + i];
  }
  g_output[dispatch_id.xy] = result;
}

[numthreads(1, GROUP_THREAD_NUM, 1)]
void CSVerticalBlur(
  int3 group_thread_id : SV_GroupThreadID,
  int3 dispatch_id : SV_DispatchThreadID)
{
  // First, fill g_cache

  // Let begin and end RADIUS threads do some extra work so that begin and end  
  // RADIUS g_cache will be initialized.
  if (group_thread_id.y < RADIUS) {
    int y = max(dispatch_id.y - RADIUS, 0);
    g_cache[group_thread_id.y] = g_input[int2(dispatch_id.x, y)];  // TODO: this is different to book.
  }

  if (group_thread_id.y >= GROUP_THREAD_NUM - RADIUS) {
    int y = min(dispatch_id.y + RADIUS, g_input.Length.y - 1);
    g_cache[group_thread_id.y + 2 * RADIUS] = g_input[int2(dispatch_id.x, y)];
  }

  g_cache[group_thread_id.y + RADIUS] =
    g_input[min(dispatch_id.xy, g_input.Length.xy - int2(0, 1))];  // TODO: this is different to book.  

  // Wait for all threads to finish.
  GroupMemoryBarrierWithGroupSync();

  // Second, do blur

  float4 result;
  for (int i = -RADIUS; i <= RADIUS; i++) {
    result += g_weights[i + RADIUS] * g_cache[group_thread_id.y + RADIUS + i];
  }
  g_output[dispatch_id.xy] = result;
}

technique11 HorizonalBlur
{
  pass p
  {
    SetVertexShader(NULL);
    SetComputeShader(CompileShader(cs_5_0, CSHorizonalBlur()));
    SetPixelShader(NULL);
  }
}

technique11 VerticalBlur
{
  pass p
  {
    SetVertexShader(NULL);
    SetComputeShader(CompileShader(cs_5_0, CSVerticalBlur()));
    SetPixelShader(NULL);
  }
}