#version 130
uniform float outsideCircle;
uniform float insideCircle;
void main()
{
	vec2 coords = gl_TexCoord[0].st;
	vec2 center = vec2(0.5, 0.5);
	float dist = distance(center, coords);
	float maxDist = 0.5 * outsideCircle;
	float minDist = 0.5 * insideCircle;
	if (dist > minDist && dist < maxDist) {
		gl_FragColor = vec4(0.7, 0.2, 0.7, 1.0);
	}
	else {
		gl_FragColor = vec4(0.0, 0.0, 0.0, 0.0);
	}
}