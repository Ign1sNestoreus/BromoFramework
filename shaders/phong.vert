#version 450

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoord;

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
    vec2 texOffset;
    float time;
    float pad4;
} ubo;

layout(location = 0) out vec3 fragPos;
layout(location = 1) out vec3 fragNormal;
layout(location = 2) out vec2 fragTexCoord;
layout(location = 3) out vec3 fragLightPos;
layout(location = 4) out vec3 fragViewPos;
layout(location = 5) out vec3 fragLightColor;

void main() {
    float amplitude = 0.1; // общая амплитуда, можно регулировать

    // Генерируем уникальные частоты на основе координат вершины
    float freqX = 2.0 + sin(inPosition.x * 5.0) * 1.5;
    float freqY = 2.5 + cos(inPosition.y * 5.0) * 1.5;
    float freqZ = 1.8 + sin(inPosition.z * 5.0) * 1.5;

    // Фазовые сдвиги тоже от координат
    float phaseX = inPosition.x * 8.0;
    float phaseY = inPosition.y * 8.0;
    float phaseZ = inPosition.z * 8.0;

    // Смещения по каждой оси
    float offsetX = sin(ubo.time * freqX + phaseX) * amplitude;
    float offsetY = cos(ubo.time * freqY + phaseY) * amplitude;
    float offsetZ = sin(ubo.time * freqZ + phaseZ) * amplitude;

    vec3 displacedPos = inPosition + vec3(offsetX, offsetY, offsetZ);

    // Стандартные преобразования
    gl_Position = ubo.proj * ubo.view * ubo.model * vec4(displacedPos, 1.0);
    fragPos = vec3(ubo.model * vec4(displacedPos, 1.0));
    fragNormal = mat3(transpose(inverse(ubo.model))) * inNormal;
    fragTexCoord = inTexCoord + ubo.texOffset;
    //fragTexCoord = inTexCoord;
    fragLightPos = ubo.lightPos;
    fragViewPos = ubo.viewPos;
    fragLightColor = ubo.lightColor;
}
