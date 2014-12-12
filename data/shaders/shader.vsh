
void main()
{
	vec4 clipCoord = gl_ModelViewProjectionMatrix * gl_Vertex;
	gl_Position = clipCoord;

	vec4 color = vec4(gl_Normal, 1.0);
	gl_FrontColor = color;
	gl_BackColor = color;

	gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_TexCoord[1] = gl_MultiTexCoord0;
	gl_TexCoord[2] = gl_MultiTexCoord0;
}
