uniform sampler2D textureSampler;

in vec4 color;

void main()
{
	gl_FragColor = texture2D(textureSampler, gl_TexCoord[0].st) * color;
}