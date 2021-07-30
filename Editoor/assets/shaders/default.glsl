#type vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 color;

uniform mat4 viewProjection;
uniform mat4 transform;

out vec3 vPosition;
out vec4 vColor;
			
void main() {
	vPosition = position;
	vColor = color;
	gl_Position = viewProjection * transform * vec4(position, 1.0);
}

#type pixel
#version 330 core

layout(location = 0) out vec4 color;
in vec3 vPosition;
in vec4 vColor;

void main() {
	color = vec4(vPosition * 0.5 + 0.5, 1.0);
	color = vColor;
}