#pragma once

static const char verticesSource[] = R"(
#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 TexCoord;

void main() {
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    TexCoord = aTexCoord;
}
)";

static const char fragmentSource[] = R"(
#version 330 core

out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D texture1;
uniform vec4 color;

void main() {
    FragColor = texture(texture1, TexCoord) * color;
}
)";

static const char fontVerticesSource[] = R"(
#version 330 core

layout (location = 0) in vec4 vertex; // <vec2 pos, vec2 tex>

out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * vec4(vertex.xy, 0.0, 1.0);
    TexCoords = vertex.zw;
}
)";

static const char fontFragmentSource[] = R"(
#version 330 core

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D text;
uniform vec4 color;

void main()
{    
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);
    if(sampled.a < 0.1)
        discard;
    FragColor = color * sampled;
}
)";

static const char cubeVerticesSource[] = R"(
#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;
layout (location = 2) in vec3 aNormal;

out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    TexCoords = aTexCoords;
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}
)";

static const char cubeFragmentSource[] = R"(
#version 330 core

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture1;
uniform vec4 color;

void main()
{   
    vec4 tex = texture(texture1, TexCoords);

    if (tex.x == 0.0 && tex.y == 0 && tex.z == 0) {
        FragColor = color;
    } else {
        FragColor = tex * color;
    }
}
)";
