#type vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 color;
layout(location = 2) in vec2 texCoords;
layout(location = 3) in float texIndex;
layout(location = 4) in float tilingFactor;

uniform mat4 viewProjection;

out vec2 vTexCoords;
out vec4 vColor;
flat out float vTexIndex;
out float vTilingFactor;
			
void main() {
	vTexCoords = texCoords;
	vColor = color;
	vTexIndex = texIndex;
	vTilingFactor = tilingFactor;
	gl_Position = viewProjection * vec4(position, 1.0);
}

#pragma #type fragment
#version 330 core

in vec4 vColor;
in vec2 vTexCoords;
flat in float vTexIndex;
in float vTilingFactor;

out vec4 oColor;

uniform sampler2D _textures[32];

void main() {
	oColor = texture(_textures[int(vTexIndex)], vTexCoords * vTilingFactor) * vColor;
	//oColor = vec4(vTexCoords, 0.5f, 1.0f);
}