#type vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoords;

uniform mat4 viewProjection;
uniform mat4 transform;
			
void main() {
	gl_Position = viewProjection * transform * vec4(position, 1.0);
}

#type fragment
#version 330 core

out vec4 oColor;

uniform vec3 color;

void main() {
	oColor = vec4(color, 1.0f);
}