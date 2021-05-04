#version 430 core

#extension GL_NV_shader_buffer_load : enable

layout (location = 0) in vec3 aPos;
layout(location = 1) in vec3 aTexCoords;
layout(location = 2) in vec2 NormalCoods;

layout(std430, binding = 1) buffer ShaderStorageBufferObject {
    double PositionArray[];
};

uniform mat4 Transform;
uniform mat4 Camera;
uniform mat4 Projection;

out vec4 VertexColor;
out vec2 TexCoords;

void main() {

	TexCoords = vec2(aTexCoords.x,1 - aTexCoords.y);

	vec4 newCoords = vec4(PositionArray[3*gl_InstanceID],PositionArray[3*gl_InstanceID+1],PositionArray[3*gl_InstanceID+2],0.0);

    gl_Position = Projection * Camera * Transform * (newCoords + vec4(aPos, 1.0));
	//(PositionArray[gl_InstanceID] +

}