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
layout (location = 3) in ivec4 aBoneIDs;
layout (location = 4) in vec4 aWeights;

out vec4 pos;
out vec2 TexCoords;
out vec3 normal;

const int MAX_BONES = 100;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 global;
uniform mat4 bones[MAX_BONES];
uniform mat4 globals[MAX_BONES];

void main() {
    TexCoords = aTexCoords;
    
    if ((aWeights[0] + aWeights[1] + aWeights[2] + aWeights[3]) == 0.0f) {
        pos = model * global * vec4(aPos, 1.0);
    } else {
        mat4 boneTransform = bones[aBoneIDs[0]] * aWeights[0];
        boneTransform += bones[aBoneIDs[1]] * aWeights[1];
        boneTransform += bones[aBoneIDs[2]] * aWeights[2];
        boneTransform += bones[aBoneIDs[3]] * aWeights[3];
        pos = model * boneTransform * vec4(aPos, 1.0);
    }

    pos = model * global * vec4(aPos, 1.0);
    gl_Position = projection * view  * pos;
    vec4 bNormal = vec4(aNormal, 0.0);
    normal = mat3(transpose(inverse(model))) * bNormal.xyz;
}
)";

static const char fragmentSource[] = R"(
#version 330 core

out vec4 FragColor;

in vec4 pos;
in vec2 TexCoords;
in vec3 normal;

uniform sampler2D texture1;
uniform samplerCube textureEnv;
uniform vec4 color;

uniform vec3 lightPos[10];
uniform vec3 lightColor[10];
uniform vec3 cameraPos;

uniform bool gamma;
uniform bool blinn;
uniform bool useLight;
uniform bool useReflect;
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

        FragColor = vec4(result, mcolor.a);
    } else if (useReflect) {
        vec3 I = normalize(pos.xyz - cameraPos);
        vec3 R = reflect(I, normalize(normal));
        FragColor = vec4(texture(textureEnv, R).rgb, 1.0);
    } else {
        FragColor.rgb = mcolor.rgb;
        FragColor.a = 1.0;
    }
}
)";
