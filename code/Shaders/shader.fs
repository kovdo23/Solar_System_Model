#version 400 core
out vec4 FragColor;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct Light {
    vec3 direction;
    vec3 position;  
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;

    float cutOff;
    float outerCutOff;
};

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform vec3 viewPos;
uniform Material material;
uniform Light light;

void main() {
    // 1. Alapvető vektorok és távolság
    vec3 lightDir = normalize(light.position - FragPos);
    float distance = length(light.position - FragPos);
    
    // 2. Távolsági csillapodás (Attenuation)
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    

    // 3. Fénykúp lágy pereme (Spotlight intensity)
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);    

    // 4. Környezeti fény (Ambient)
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
        
    // 5. Szórt fény (Diffuse)
    vec3 norm = normalize(Normal);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords)); 
    
    // 6. Tükröződés (Specular)
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));

    // 7. A Diffuse és Specular fények halványítása a távolság és a fénykúp alapján
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;

    // 8. Végeredmény (Az Ambient fényt szándékosan nem halványítjuk, így nem lesz minden koromfekete)
    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}