#version 330 core

out vec4 FragColor;

in vec3 FragPos;
in vec3 normal;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor;
uniform vec3 objectColor;
uniform vec4 materialValues; // ambient, diffuse, specular, shininess

uniform float constant;  // Constant attenuation factor
uniform float linear;    // Linear attenuation factor
uniform float quadratic; // Quadratic attenuation factor

void main()
{
    float distance = length(lightPos - FragPos);
    
    float attenuation = 1.0 / (constant + linear * distance + quadratic * (distance * distance));

    vec3 ambient = materialValues.x * lightColor;

    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor * materialValues.y;

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), materialValues.w);
    vec3 specular = materialValues.z * spec * lightColor;

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    vec3 result = (ambient + diffuse + specular) * objectColor;

    FragColor = vec4(result, 1.0);
}
