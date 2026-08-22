void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
  // draw an elipse 
  // just leave x and y have two different scaling
  vec2 uv = fragCoord/iResolution.xy;
  
   if(length(uv - vec2(0.5,0.5))<=0.2)
   {
    // now just fill the color inside
    fragColor = vec4(1.0,0.0,0.0,1.0);
   }
   else
   {
   // just leave everything black
    fragColor = vec4(0.0,0.0,0.0,1.0);
   }
   
}