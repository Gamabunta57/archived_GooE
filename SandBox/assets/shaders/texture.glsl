#type vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 color;
layout(location = 2) in vec2 texCoords;

uniform mat4 viewProjection;

out vec2 vTexCoords;
out vec4 vColor;
			
void main() {
	vTexCoords = texCoords;
	vColor = color;
	gl_Position = viewProjection * vec4(position, 1.0);
}

#pragma #type fragment
#version 330 core

in vec4 vColor;
in vec2 vTexCoords;

out vec4 oColor;

uniform sampler2D _texture;
uniform float _tilingFactor;
uniform vec4 _tint;

void main() {
	//oColor = texture(_texture, vTexCoords * _tilingFactor) * color;
	oColor = vColor;
}