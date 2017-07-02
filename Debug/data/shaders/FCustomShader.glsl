#version 130
uniform sampler2D tex;
void main()
{
	vec4 color = texture2D(tex,gl_TexCoord[0].st);
	color = color * 0.5;
	color[2] = color[2] * 2.0;
	color[3] = 1.0;
	gl_FragColor = color;
}