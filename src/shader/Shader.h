#pragma once

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

static const char verticesSource[] = R"(
#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;
layout (location = 2) in vec3 aNormal;

out vec4 pos;
out vec2 TexCoords;
out vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    TexCoords = aTexCoords;
    pos = model * vec4(aPos, 1.0);
    gl_Position = projection * view * pos;
    normal = mat3(transpose(inverse(model))) * aNormal;
}
)";

static const char fragmentSource[] = R"(
#version 330 core

out vec4 FragColor;

in vec4 pos;
in vec2 TexCoords;
in vec3 normal;

uniform sampler2D texture1;
uniform vec4 color;

uniform vec3 lightPos[10];
uniform vec3 lightColor[10];
uniform vec3 cameraPos;

uniform bool gamma;
uniform bool blinn;
uniform bool useLight;
uniform int lightSize;

vec3 BlinnPhong(vec3 normal, vec3 pos, vec3 lightPos, vec3 lightColor, bool blinn)
{
    float ambientStrength = 0.2;
    vec3 ambient = ambientStrength * vec3(1.0f, 1.0f, 1.0f);

    // diffuse
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lightPos - pos.xyz);
    float diff = max(dot(lightDir, norm), 0.0);
    vec3 diffuse = diff * lightColor;

    float specularStrength = 1.0;
    vec3 viewDir = normalize(cameraPos - pos.xyz);
    float spec = 0.0;

    if (blinn) {
        vec3 halfwayDir = normalize(lightDir + viewDir);
        spec = pow(max(dot(normal, halfwayDir), 0.0), 4.0);
    } else {
        vec3 reflectDir = reflect(-lightDir, norm);
        spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
    }

    vec3 specular = specularStrength * spec * lightColor;

    // simple attenuation
    float max_distance = 1.5;
    float distance = length(lightPos - pos.xyz);
    float attenuation = 1.0 / (gamma ? distance * distance : distance);

    diffuse *= attenuation;
    specular *= attenuation;

    return ambient + diffuse + specular;
}

void main()
{   
    vec4 tex = texture(texture1, TexCoords);
    vec4 mcolor;

    if (tex.x == 0.0 && tex.y == 0 && tex.z == 0) { 
        mcolor = color;
    } else {
        mcolor = tex * color;
    }

    if (useLight) {
        vec3 lighting = vec3(0.0);

        for(int i = 0; i < lightSize; ++i)
            lighting += BlinnPhong(normal, pos.xyz, lightPos[i], lightColor[i], blinn);
        // vec3 specular = vec3(2.0) * spec;
        vec3 result = lighting * mcolor.rgb;

        if (gamma)
            result = pow(result, vec3(1.0/2.2));

        FragColor = vec4(result, color.a);
    } else {
        FragColor = mcolor;
    }
}
)";
