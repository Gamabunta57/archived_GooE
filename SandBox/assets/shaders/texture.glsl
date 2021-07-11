#type vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoords;

uniform mat4 viewProjection;
uniform mat4 transform;

out vec2 vTexCoords;
			
void main() {
	vTexCoords = texCoords;
	gl_Position = viewProjection * transform * vec4(position, 1.0);
}

#pragma #type fragment
#version 330 core

out vec4 oColor;
in vec2 vTexCoords;

uniform sampler2D _texture;

void main() {
	oColor = texture(_texture, vTexCoords);
}