
#define NUM_LIGHTS 8

#define POINT_LIGHT 0
#define SPOT_LIGHT 1
#define DIRECTIONAL_LIGHT 2

struct Material
{
    float4  GlobalAmbient;
    //-------------------------- ( 16 bytes )
    float4  AmbientColor;
    //-------------------------- ( 16 bytes )
    float4  EmissiveColor;
    //-------------------------- ( 16 bytes )
    float4  DiffuseColor;
    //-------------------------- ( 16 bytes )
    float4  SpecularColor;
    //-------------------------- ( 16 bytes )
    // Reflective value.
    float4  Reflectance;
    //-------------------------- ( 16 bytes )
    float   Opacity;
    float   SpecularPower;
    // For transparent materials, IOR > 0.
    float   IndexOfRefraction;
    bool    HasAmbientTexture;
    //-------------------------- ( 16 bytes )
    bool    HasEmissiveTexture;
    bool    HasDiffuseTexture;
    bool    HasSpecularTexture;
    bool    HasSpecularPowerTexture;
    //-------------------------- ( 16 bytes )
    bool    HasNormalTexture;
    bool    HasBumpTexture;
    bool    HasOpacityTexture;
    float   BumpIntensity;
    //-------------------------- ( 16 bytes )
    float   SpecularScale;
    float   AlphaThreshold;
    float   HdrBrightness;
    float   Padding;
    //--------------------------- ( 16 bytes )
};

struct Light
{
    float4   PositionWS;
    float4   DirectionWS;
    float4   PositionVS;
    float4   DirectionVS;
    float4   Color;
    float    SpotlightAngle;
    float    Range;
    float    Intensity;
    bool    Enabled;
    bool    Selected;
    uint    Type;
    float2  Padding;
};

float3 ExpandNormal(float3 n)
{
    return n * 2.0f - 1.0f;
}

float4 DoNormalMapping(float3x3 TBN, Texture2D tex, sampler s, float2 uv)
{
    float3 normal = tex.Sample(s, uv).xyz;
    normal = ExpandNormal(normal);

    // Transform normal from tangent space to view space.
    normal = mul(normal, TBN);
    return normalize(float4(normal, 0));
}

float4 DoBumpMapping(float3x3 TBN, Texture2D tex, sampler s, float2 uv, float bumpScale)
{
    // Sample the heightmap at the current texture coordinate.
    float height = tex.Sample(s, uv).r * bumpScale;
    // Sample the heightmap in the U texture coordinate direction.
    float heightU = tex.Sample(s, uv, int2(1, 0)).r * bumpScale;
    // Sample the heightmap in the V texture coordinate direction.
    float heightV = tex.Sample(s, uv, int2(0, 1)).r * bumpScale;

    float3 p = { 0, 0, height };
    float3 pU = { 1, 0, heightU };
    float3 pV = { 0, 1, heightV };

    // normal = tangent x bitangent
    float3 normal = cross(normalize(pU - p), normalize(pV - p));

    // Transform normal from tangent space to view space.
    normal = mul(normal, TBN);

    return float4(normal, 0);
}

struct LightingResult
{
    float4 Diffuse;
    float4 Specular;
};

float4 DoDiffuse(Light light, float4 L, float4 N)
{
    float NdotL = max(dot(N, L), 0);
    return light.Color * NdotL;
}

float4 DoSpecular(Light light, Material material, float4 V, float4 L, float4 N)
{
    float4 R = normalize(reflect(-L, N));
    float RdotV = max(dot(R, V), 0);

    return light.Color * pow(RdotV, material.SpecularPower);
}

float DoAttenuation(Light light, float d)
{
    return 1.0f - smoothstep(light.Range * 0.75f, light.Range, d);
}

LightingResult DoPointLight(Light light, Material mat, float4 V, float4 P, float4 N)
{
    LightingResult result;

    float4 L = light.PositionVS - P;
    float distance = length(L);
    L = L / distance;

    float attenuation = DoAttenuation(light, distance);

    result.Diffuse = DoDiffuse(light, L, N) *
        attenuation * light.Intensity;
    result.Specular = DoSpecular(light, mat, V, L, N) *
        attenuation * light.Intensity;

    return result;
}

float DoSpotCone(Light light, float4 L)
{
    // If the cosine angle of the light's direction 
    // vector and the vector from the light source to the point being 
    // shaded is less than minCos, then the spotlight contribution will be 0.
    float minCos = cos(radians(light.SpotlightAngle));
    // If the cosine angle of the light's direction vector
    // and the vector from the light source to the point being shaded
    // is greater than maxCos, then the spotlight contribution will be 1.
    float maxCos = lerp(minCos, 1, 0.5f);
    float cosAngle = dot(light.DirectionVS, -L);
    // Blend between the minimum and maximum cosine angles.
    return smoothstep(minCos, maxCos, cosAngle);
}

LightingResult DoDirectionalLight(Light light, Material mat, float4 V, float4 P, float4 N)
{
    LightingResult result;

    float4 L = normalize(-light.DirectionVS);

    result.Diffuse = DoDiffuse(light, L, N) * light.Intensity;
    result.Specular = DoSpecular(light, mat, V, L, N) * light.Intensity;

    return result;
}

LightingResult DoSpotLight(Light light, Material mat, float4 V, float4 P, float4 N)
{
    LightingResult result;

    float4 L = light.PositionVS - P;
    float distance = length(L);
    L = L / distance;

    float attenuation = DoAttenuation(light, distance);
    float spotIntensity = DoSpotCone(light, L);

    result.Diffuse = DoDiffuse(light, L, N) *
        attenuation * spotIntensity * light.Intensity;
    result.Specular = DoSpecular(light, mat, V, L, N) *
        attenuation * spotIntensity * light.Intensity;

    return result;
}

LightingResult DoLighting(StructuredBuffer<Light> lights, Material mat, float4 eyePos, float4 P, float4 N)
{
    float4 V = normalize(eyePos - P);

    LightingResult totalResult = (LightingResult)0;

    for (int i = 0; i < NUM_LIGHTS; ++i)
    {
        LightingResult result = (LightingResult)0;

        Light lll = lights[i];

        // Skip lights that are not enabled.
        if (lights[i].Enabled == 0) continue;
        // Skip point and spot lights that are out of range of the point being shaded.
        if (lights[i].Type != DIRECTIONAL_LIGHT &&
            length(lights[i].PositionVS - P) > lights[i].Range) continue;

        switch (lights[i].Type)
        {
        case DIRECTIONAL_LIGHT:
        {
            result = DoDirectionalLight(lights[i], mat, V, P, N);
        }
        break;
        case POINT_LIGHT:
        {
            result = DoPointLight(lights[i], mat, V, P, N);
        }
        break;
        case SPOT_LIGHT:
        {
            result = DoSpotLight(lights[i], mat, V, P, N);
        }
        break;
        }
        totalResult.Diffuse += result.Diffuse;
        totalResult.Specular += result.Specular;
    }

    return totalResult;
}