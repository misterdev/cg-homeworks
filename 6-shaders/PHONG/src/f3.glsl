varying vec3 N;
varying vec3 L;
varying vec3 E;
varying vec3 R;

void main() {
    vec3 Normal = normalize(N);
    vec3 Light  = normalize(L);
    vec3 Eye    = normalize(E);
    vec3 Refl   = normalize(R);

    float Kd = max(dot(Normal, Light), 0.0);
    float Ks = pow(max(dot(Refl, Normal), 0.0), 0.25 * gl_FrontMaterial.shininess);
    float Ka = 0.0;

    vec4 diffuse  = Kd * gl_FrontLightProduct[0].diffuse;
    vec4 specular = Ks * gl_FrontLightProduct[0].specular;
    vec4 ambient  = Ka * gl_FrontLightProduct[0].ambient;

    gl_FragColor = ambient + diffuse + specular;
}

//F3
//float ks = pow(max(dot(refl, eye), 0.0), 0.25*gl_FrontMaterial.shininess)
// qui cambia solo la riflettivita'