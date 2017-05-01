uniform sampler2D textureSampler;
uniform float coeficient;

in vec4 color;

void main()
{
	gl_FragColor = texture2D(textureSampler, gl_TexCoord[0].st) * color * coeficient;
}