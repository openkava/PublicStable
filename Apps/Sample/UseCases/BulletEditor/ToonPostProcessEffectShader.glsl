
// Image texture.
uniform sampler2D u_rgbaImage;
uniform sampler2D u_normalImage;

/*
// Width of Image.
uniform int u_width;
// Height of Image.
uniform int u_height;
*/

void main()
{
  gl_FragColor = texture2D( u_rgbaImage, gl_TexCoord[0].st) * 0.5 +
                 texture2D( u_normalImage, gl_TexCoord[0].st) * 0.5;
}

