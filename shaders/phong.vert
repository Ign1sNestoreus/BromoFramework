#version 450

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;

layout(binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 proj;
    vec3 lightPos;
    float pad1;
    vec3 lightColor;
    float pad2;
    vec3 viewPos;
    float pad3;
} ubo;

layout(location = 0) out vec3 fragPos;
layout(location = 1) out vec3 fragNormal;
layout(location = 2) out vec3 fragLightPos;
layout(location = 3) out vec3 fragViewPos;
layout(location = 4) out vec3 fragLightColor;

void main() {
    gl_Position = ubo.proj * ubo.view * ubo.model * vec4(inPosition, 1.0);
    fragPos = vec3(ubo.model * vec4(inPosition, 1.0));
    fragNormal = mat3(transpose(inverse(ubo.model))) * inNormal; // преобразование нормали
    fragLightPos = ubo.lightPos;
    fragViewPos = ubo.viewPos;
    fragLightColor = ubo.lightColor;
}
