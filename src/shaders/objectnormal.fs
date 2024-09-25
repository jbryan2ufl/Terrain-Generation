// #version 330 core

// out vec4 FragColor;

// in vec3 flatNormal;
// in vec3 flatColor;

// void main() {
//     FragColor = vec4(flatNormal, 1.0);
// }





#version 330 core

out vec4 FragColor;

in vec3 FragPos;
in vec3 normal;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor;
uniform vec3 objectColor;
uniform vec4 materialValues; // ambient, diffuse, specular, shininess

uniform float constant; // Constant attenuation factor
uniform float linear;   // Linear attenuation factor
uniform float quadratic; // Quadratic attenuation factor

void main()
{
    // Calculate distance to the light source
    float distance = length(lightPos - FragPos);
    
    // Calculate attenuation
    float attenuation = 1.0 / (constant + linear * distance + quadratic * (distance * distance));

    // Ambient
    vec3 ambient = materialValues.x * lightColor;

    // Diffuse
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor * materialValues.y;

    // Specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), materialValues.w);
    vec3 specular = materialValues.z * spec * lightColor;

    // Apply attenuation to the lighting components
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    // Combine results
    vec3 result = (ambient + diffuse + specular) * objectColor;
    FragColor = vec4(result, 1.0);
}













// #version 330 core

// out vec4 FragColor;

// in vec3 FragPos;
// in vec3 normal;

// uniform vec3 lightPos;
// uniform vec3 viewPos;
// uniform vec3 lightColor;
// uniform vec3 objectColor;
// uniform vec4 materialValues; // ambient, diffuse, specular, shininess

// void main()
// {
//     vec3 ambient = materialValues.x * lightColor;

//     vec3 norm = normalize(normal);
//     vec3 lightDir = normalize(lightPos - FragPos);
//     float diff = max(dot(norm, lightDir), 0.0);
//     vec3 diffuse = diff * lightColor * materialValues.y;

//     vec3 viewDir = normalize(viewPos - FragPos);
//     vec3 reflectDir = reflect(-lightDir, norm);
//     float spec = pow(max(dot(viewDir, reflectDir), 0.0), materialValues.w);
//     vec3 specular = materialValues.z * spec * lightColor;

//     vec3 result = (ambient + diffuse + specular) * objectColor;
//     FragColor = vec4(result, 1.0);
// }