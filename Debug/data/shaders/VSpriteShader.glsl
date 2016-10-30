out vec4 color;

void main()
{
	gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * gl_Vertex;
	color = normalize(gl_Position);
	gl_TexCoord[0] = gl_MultiTexCoord0;
}