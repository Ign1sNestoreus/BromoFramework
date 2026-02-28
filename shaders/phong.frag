#version 450

layout(location = 0) in vec3 fragPos;
layout(location = 1) in vec3 fragNormal;
layout(location = 2) in vec3 fragLightPos;
layout(location = 3) in vec3 fragViewPos;
layout(location = 4) in vec3 fragLightColor;

layout(location = 0) out vec4 outColor;

void main() {
    // Материал (можно тоже передавать через uniform, но для простоты зададим)
    vec3 objectColor = vec3(0.6, 0.8, 1.0); // голубоватый
    float ambientStrength = 0.1;
    float diffuseStrength = 1.0;
    float specularStrength = 0.5;
    float shininess = 32.0;

    // Ambient
    vec3 ambient = ambientStrength * fragLightColor;

    // Diffuse
    vec3 norm = normalize(fragNormal);
    vec3 lightDir = normalize(fragLightPos - fragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diffuseStrength * diff * fragLightColor;

    // Specular (Blinn-Phong)
    vec3 viewDir = normalize(fragViewPos - fragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(norm, halfwayDir), 0.0), shininess);
    vec3 specular = specularStrength * spec * fragLightColor;

    vec3 result = (ambient + diffuse + specular) * objectColor;
    outColor = vec4(result, 1.0);
}
