void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
  vec2 uv = fragCoord/iResolution.xx;
  
  vec2 center = vec2(0.5,(iResolution.y/iResolution.x)*0.5);
  
  if(length(uv - center)<=0.2)
  {
    fragColor = vec4(1.0,0.0,0.0,1.0);
  }
  else
  {
   fragColor = vec4(0.0,0.0,0.0,1.0);
  }
  
}