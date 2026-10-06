#version 460

// Input vertex attributes (from vertex shader)
in vec3 fragPosition;
in vec2 fragTexCoord;
in vec4 fragColor;
in vec3 fragNormal;

// Input uniform values
uniform sampler2D texture0;
uniform vec4 colDiffuse;

// Output fragment color
out vec4 finalColor;

// NOTE: Add your custom variables here

#define     MAX_LIGHTS              4
#define     LIGHT_DIRECTIONAL       0
#define     LIGHT_POINT             1

struct Light {
    int enabled;
    int type;
    vec3 position;
    vec3 target;
    vec4 color;
};

// Input lighting values
uniform Light lights[MAX_LIGHTS];
uniform vec4 ambient;
uniform vec3 viewPos;

uniform float materialShininess; // Higher = tighter specular highlight
uniform float materialGloss;     // 0.0 = completely matte, 1.0 = shiny reflections
uniform float materialAlpha;     // Transparency value (0.0 to 1.0)
uniform float lightIntensity;


void main()
{
    vec4 texelColor = texture(texture0, fragTexCoord);
    vec3 lightDot = vec3(0.0);
    vec3 normal = normalize(fragNormal);
    vec3 viewD = normalize(viewPos - fragPosition);
    vec3 specular = vec3(0.0);

    // Apply custom alpha transparency directly to the base tint color
    vec4 tint = colDiffuse * fragColor;
    tint.a *= materialAlpha; 

    for (int i = 0; i < MAX_LIGHTS; i++)
    {
        if (lights[i].enabled == 1)
        {
            vec3 light = vec3(0.0);
            if (lights[i].type == LIGHT_DIRECTIONAL) light = -normalize(lights[i].target - lights[i].position);
            if (lights[i].type == LIGHT_POINT)       light = normalize(lights[i].position - fragPosition);

            float NdotL = max(dot(normal, light), 0.0);
            
            // Multiply the light color by your intensity uniform
            vec3 effectiveColor = lights[i].color.rgb * lightIntensity;
            lightDot += effectiveColor * NdotL;

            float specCo = 0.0;
            if (NdotL > 0.0) 
            {
                // Scale the specular highlight brightness by intensity as well
                specCo = pow(max(0.0, dot(viewD, reflect(-(light), normal))), materialShininess) * materialGloss * lightIntensity;
            }
            specular += specCo;
        }
    }

    // --- FIXED COLOR MATH ---
    // Calculate diffuse and specular lighting strictly on the RGB channels
    vec3 diffuseEffect = tint.rgb * lightDot;
    vec3 specularEffect = specular;
    
    // Ambient fallback illumination (RGB only)
    vec3 ambientEffect = (ambient.rgb / 10.0) * tint.rgb;

    // Combine color systems, factor in the texture, and preserve the explicit alpha channel
    finalColor.rgb = texelColor.rgb * (diffuseEffect + specularEffect + ambientEffect);
    finalColor.a = texelColor.a * tint.a; 

    // Gamma correction
    finalColor = pow(finalColor, vec4(1.0 / 2.2));
}

