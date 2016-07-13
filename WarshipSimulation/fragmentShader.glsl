#version 330 core
in vec3 TexCoords;

in vec4 color;
out vec4 fragColor;

bool isTexture;

uniform samplerCube skybox;

void main()	{
	//printf("1");
	if(isTexture)
		fragColor = texture(skybox, TexCoords);
	else
		fragColor = color;
}