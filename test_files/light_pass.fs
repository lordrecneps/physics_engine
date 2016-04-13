#version 330

uniform vec3 lightPos;
uniform vec3 lightCol;
uniform float ambient;
uniform float attenuation;

uniform vec3 cameraPos;

uniform sampler2D posBuff;
uniform sampler2D colorBuff;
uniform sampler2D normalBuff;
uniform samplerCube shadowMap;
uniform vec2 screenDim;

ivec2 shadowDim = textureSize(shadowMap, 0);
float shadowOffset = 1.0f / shadowDim.x;

vec3 CalcPointLight(vec3 pos, vec3 N, vec3 objColor)
{
    vec3 L = lightPos - pos;
    float dist = length(L);
	
	L = L / dist;
	
	float LdotN = max(dot(L, N), 0);
	float spec = 0.0;
	float shadowVal = 0.0f;
	float dropoff =  1.0 + attenuation * dist * dist;
	
	if (LdotN > 0)
	{
		vec3 mL = -L;
		
		vec3 aL = abs(L);
		vec3 xDir, yDir;
		
		if(aL.x > aL.y)
		{
			xDir = normalize(vec3(-mL.z, 0, mL.x));
			yDir = cross(mL, xDir);
		}
		else
		{
			xDir = normalize(vec3(0, -mL.z, mL.y));
			yDir = cross(mL, xDir);
		}
		
		for (int i = -1; i <= 1; ++i)
		{
			for (int j = -1; j <= 1; ++j)
			{
				float sDist = texture(shadowMap, mL + xDir * shadowOffset * i + yDir * shadowOffset * j).r;
				shadowVal += float(dist < (sDist + 0.005*dist + 0.02*(1 - LdotN)*dist));
			}
		}
		
		shadowVal *= 0.11111111111111111111f;
		
		//if (dist >= (SampledDistance + 0.01))
		//    return objColor * ambient / dropoff * lightCol; // Inside the shadow
		//shadowVal = 1.0f;
    
		vec3 V = normalize(cameraPos - pos);
		vec3 H = normalize(V + L);
		float NdotH = max(dot(N, H), 0);
		spec = pow(NdotH, 100.0f);
	}
	
    vec3 color = ((objColor*(LdotN*shadowVal + ambient) + spec*vec3(1.0, 1.0, 1.0)*shadowVal) / dropoff) * lightCol;

    return color;
	//return vec3(shadowVal);
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
