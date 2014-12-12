
uniform sampler2D texUnit0;

void main()
{
	gl_FragColor = texture2D(texUnit0, gl_TexCoord[0].xy);
	//gl_FragColor = vec4(gl_TexCoord[0].x, gl_TexCoord[0].y, 0, 1);
}
