#version 430 core

in vec4 VertexColor;
in vec2 TexCoords;

out vec4 FragColor;

uniform sampler2D Texture;
uniform sampler2D Texture2;

void main() {

	//FragColor = mix(texture(Texture, TexCoords), texture(Texture2, TexCoords), 0.5);
	FragColor = texture(Texture, TexCoords);
}