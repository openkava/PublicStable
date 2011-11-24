
// Image texture.
uniform sampler2D u_colorImage;
uniform sampler2D u_normalImage;
uniform sampler2D u_depthImage;
uniform int u_width;
uniform int u_height;
uniform float u_outlineWidth;
uniform vec4 u_outlineColor;

void main()
{
  vec2 tc_offset[9];
  float xInc = u_outlineWidth / float(u_width);
  float yInc = u_outlineWidth / float(u_height);

  for(int i=0;i<3;i++) {
    for(int j=0;j<3;j++) {
      tc_offset[((i*3)+j)] = vec2((-1.0 * xInc) + ( float(i) * xInc), (-1.0 * yInc) + ( float(j) * yInc));
    }
  }
  
  vec4 normalSample[9];
  float depthSample[9];
  for (int i = 0; i < 9; i++){
    vec2 uv = gl_TexCoord[0].st + tc_offset[i];
    normalSample[i] = texture2D(u_normalImage, uv);
    depthSample[i] = texture2D(u_depthImage, uv).z;
  }
  
  vec4 normalHorizEdge = normalSample[2] + (2.0*normalSample[5]) + normalSample[8] - (normalSample[0] + (2.0*normalSample[3]) + normalSample[6]);
  vec4 normalVertEdge = normalSample[0] + (2.0*normalSample[1]) + normalSample[2] - (normalSample[6] + (2.0*normalSample[7]) + normalSample[8]);
  float depthHorizEdge = depthSample[2] + (2.0*depthSample[5]) + depthSample[8] - (depthSample[0] + (2.0*depthSample[3]) + depthSample[6]);
  float depthVertEdge = depthSample[0] + (2.0*depthSample[1]) + depthSample[2] - (depthSample[6] + (2.0*depthSample[7]) + depthSample[8]);

  vec3 normalEdge = sqrt((normalHorizEdge.rgb * normalHorizEdge.rgb) + (normalVertEdge.rgb * normalVertEdge.rgb));
  float depthEdge = sqrt((depthHorizEdge * depthHorizEdge) + (depthVertEdge * depthVertEdge));
  float edge = max(depthEdge,max(max(normalEdge.r,normalEdge.g),normalEdge.b));

  gl_FragColor = edge * u_outlineColor + (1.0 - edge) * texture2D( u_colorImage, gl_TexCoord[0].st );
}

