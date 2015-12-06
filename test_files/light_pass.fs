#version 330

uniform vec3 lightPos;
uniform vec3 lightCol;
uniform float ambient;
uniform float attenuation;

uniform vec3 cameraPos;

uniform sampler2D posBuff;
uniform sampler2D colorBuff;
uniform sampler2D normalBuff;
uniform vec2 screenDim;

vec3 CalcPointLight(vec3 pos, vec3 N, vec3 objColor)
{
    vec3 L = lightPos - pos;
    float dist = length(L);
    L = L / dist;
    
    float LdotN = max(dot(L, N), 0);
	float spec = 0.0;
    
	if(LdotN > 0.0f)
	{
		vec3 V = normalize(cameraPos - pos);
		vec3 H = normalize(V + L);
		float NdotH = max(dot(N, H), 0);
		spec = pow(NdotH, 100.0f);
	}

    float dropoff =  1.0 + attenuation * dist * dist;
    vec3 color = ((objColor*(LdotN + ambient) + spec*vec3(1.0, 1.0, 1.0)) / dropoff) * lightCol;

    return color;
}


vec2 CalcTexCoord()
{
    return gl_FragCoord.xy / screenDim;
}

out vec4 finalColor;

void main()
{
    vec2 texCoord = CalcTexCoord();
	vec3 pos = texture(posBuff, texCoord).xyz;
	vec3 color = texture(colorBuff, texCoord).xyz;
	vec3 N = texture(normalBuff, texCoord).xyz;
	N = normalize(N);

    vec3 fColor = CalcPointLight(pos, N, color);
    vec3 gamma = vec3(1.0/2.2);
    finalColor = vec4(pow(fColor, gamma), 1.0);
}
