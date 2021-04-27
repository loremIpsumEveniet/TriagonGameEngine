#version 430 core

layout (location = 0) in vec3 aPos;
layout(location = 1) in vec3 aTexCoords;
layout(location = 2) in vec2 NormalCoods;

uniform mat4 Transform;
uniform mat4 Camera;
uniform mat4 Projection;

out vec4 VertexColor;
out vec2 TexCoords;

void main() {

	TexCoords = vec2(aTexCoords.x,1 - aTexCoords.y);

    gl_Position = Projection * Camera * Transform * vec4(aPos, 1.0);
}