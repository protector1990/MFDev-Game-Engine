#version 110
uniform sampler2D tex;
uniform float elapsedTime;
void main()
{
	float offsetFactor = 0.01;
	float elapsedTimeFactor = 3.0;
	vec2 coords = gl_TexCoord[0].st;
	float sinFactor = sin(coords[0] * 15.0 + elapsedTime * elapsedTimeFactor);
	float cosFactor = cos(-coords[1] * 15.0 + elapsedTime * elapsedTimeFactor);
	coords[0] += sinFactor * offsetFactor;
	coords[1] += cosFactor * offsetFactor;
	vec4 color = texture2D(tex, coords);
	gl_FragColor = color;
}