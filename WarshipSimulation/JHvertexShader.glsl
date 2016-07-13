#version 330 core
layout (location = 0) in vec3 position;
out vec3 TexCoords;

uniform mat4 projection;
uniform mat4 view;

bool isTexture;

in vec4 vPosition;
in vec4 vColor;
in vec3 vNormal;  // not used

uniform mat4 ModelViewProjection;  // = projection * view * model
out vec4 color;

void main()	{
	if(isTexture)	{
		gl_Position = projection * view * vec4(position, 1.0);
		TexCoords = position;
	}
	else {
		color = vColor;
		gl_Position = ModelViewProjection * vPosition;
	}


}